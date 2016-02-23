#include "client_socket_manager.h"

#include <chrono>

#include "utils.h"

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

bool ClientSocketManager::isIncomingBufferEmpty() {
    mutex_guard _(mContext->mIncomingMutex);
    return mContext->mIncomingBuffer.empty();
}

std::vector<char> ClientSocketManager::pop() {
    mutex_guard _(mContext->mIncomingMutex);
    assert(!mContext->mIncomingBuffer.empty());
    auto res = mContext->mIncomingBuffer.front();
    mContext->mIncomingBuffer.erase(mContext->mIncomingBuffer.begin());
    return res;
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
                   if (res && res->size())
                       ctx->mIncomingBuffer.push_back(*res);
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
