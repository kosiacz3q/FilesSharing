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
    assert(!mIncomingBuffer.empty());
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
    std::thread sender([ctx = mContext]{
       while (!ctx->mStop) {
           {
               std::lock(ctx->mOutgoingMutex, ctx->mSocketMutex);
               mutex_guard _1(ctx->mOutgoingMutex, std::adopt_lock);
               mutex_guard _2(ctx->mSocketMutex, std::adopt_lock);
               if (!ctx->mOutgoingBuffer.empty()) {
                   bool res = ctx->mSocket.send(ctx->mOutgoingBuffer.front());
                   (void) res;
                   ctx->mOutgoingBuffer.erase(ctx->mOutgoingBuffer.begin());
               }
           }
           std::this_thread::sleep_for(5ms);
       }
    });
    sender.detach();

    std::thread receiver([ctx = mContext]{
        while (!ctx->mStop) {
            {
                std::lock(ctx->mIncomingMutex, ctx->mSocketMutex);
                mutex_guard _1(ctx->mIncomingMutex, std::adopt_lock);
                mutex_guard _2(ctx->mSocketMutex, std::adopt_lock);
                auto res = ctx->mSocket.receive();
                if (res) ctx->mIncomingBuffer.push_back(*res);
            }
            std::this_thread::sleep_for(5ms);
        }
    });
    receiver.detach();
}