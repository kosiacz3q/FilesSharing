#include "client_socket_manager.h"

#include <chrono>

#include "common/utils.h"
#include "common/api.h"

ClientSocketManager::ClientSocketManager(ClientSocket&& socket)
    : mContext(std::make_shared<Context>(std::move(socket))) {
    assert(mContext->mSocket.isValid());
    loop();
}

ClientSocketManager::~ClientSocketManager() {
    stop();
}

bool ClientSocketManager::isOutgoingBufferEmpty() {
    mutex_guard _(mContext->mOutgoingMutex);
    return mContext->mOutgoingBuffer.empty();
}

static uint32_t getIncomingBufferMessageSize(const std::vector<char>& buffer) {
    uint8_t type, status;
    uint32_t id, size;
    (void) type;
    (void) status;
    (void) id;

    assert(buffer.size() >= Api::PayloadInBytesOffset);
    (void) from_bytes(buffer.begin(), buffer.end(), type, status, id, size);
    return size;
}

bool ClientSocketManager::isIncomingBufferEmpty() {
    auto& buffer = mContext->mIncomingBuffer;
    if (buffer.size() < Api::PayloadInBytesOffset)
        return true;

    auto size = getIncomingBufferMessageSize(buffer);
    if (buffer.size() < Api::PayloadInBytesOffset + size)
        return true;

    return false;
}

std::vector<char> ClientSocketManager::pop() {
    mutex_guard _(mContext->mIncomingMutex);
    assert(!mContext->mIncomingBuffer.empty());
    auto res = getFullIncomingMessage();
    assert(res);
    return *res;
}

void ClientSocketManager::push(std::vector<char> payload) {
    mutex_guard _(mContext->mOutgoingMutex);
    mContext->mOutgoingBuffer.push_back(std::move(payload));
}

void ClientSocketManager::stop() {
    mContext->mStop.store(true);
}


void ClientSocketManager::loop() {
    using namespace std::chrono_literals;
    std::thread looper([ctx = mContext]{
       while (!ctx->mStop) {
           {
               {
                   mutex_guard _(ctx->mOutgoingMutex);
                   if (!ctx->mOutgoingBuffer.empty()) {
                       auto res = ctx->mSocket.send(ctx->mOutgoingBuffer.front());
                       if (res) {
                           assert(*res == ctx->mOutgoingBuffer.front().size());
                           ctx->mOutgoingBuffer.erase(ctx->mOutgoingBuffer.begin());
                       }
                   }
               }
               {
                   mutex_guard _(ctx->mIncomingMutex);
                   auto res = ctx->mSocket.receive();
                   if (res && !res->empty())
                       ctx->mIncomingBuffer.insert(ctx->mIncomingBuffer.end(), res->begin(),
                                                   res->end());
               }
           }
           std::this_thread::sleep_for(5ms);
       }
    });
    looper.detach();
}

int ClientSocketManager::getClientId() const{
    return mContext->mSocket.getSocketFd();
}

core::optional<std::vector<char>> ClientSocketManager::getFullIncomingMessage() {
    // Assumes lock on incomingbuffer. Cannot assert for it, because it's not a unique lock.
    auto& buffer = mContext->mIncomingBuffer;
    if (buffer.size() < Api::PayloadInBytesOffset)
        return core::nullopt;

    auto size = getIncomingBufferMessageSize(buffer);
    if (buffer.size() < Api::PayloadInBytesOffset + size)
        return core::nullopt;

    auto it = buffer.begin() + Api::PayloadInBytesOffset + size;
    std::vector<char> res(buffer.begin(), it);
    buffer.erase(buffer.begin(), it);
    return res;
}