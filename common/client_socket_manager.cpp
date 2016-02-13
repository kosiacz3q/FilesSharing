#include "client_socket_manager.h"

#include <cassert>
#include <chrono>

#include "utils.h"

ClientSocketManager::ClientSocketManager(ClientSocket&& socket) {
    mContext = std::make_shared<Context>(std::move(socket));
    assert(mContext->mSocket.isValid());
    loop();
}

ClientSocketManager::~ClientSocketManager() {
    stop();
}

bool ClientSocketManager::isOutgoingBufferEmpty() {
    mutex_guard _(mContext->mOutgoingMutex);
    return mOutgoingBuffer.empty();
}

bool ClientSocketManager::isIncomingBufferEmpty() {
    mutex_guard _(mContext->mIncomingMutex);
    return mIncomingBuffer.empty();
}

std::vector<char> ClientSocketManager::pop() {
    mutex_guard _(mContext->mIncomingMutex);
    assert(!mIncomingBuffer.empty());
    auto res = mIncomingBuffer.front();
    mIncomingBuffer.erase(mIncomingBuffer.begin());
    return res;
}

void ClientSocketManager::push(std::vector<char> payload) {
    mutex_guard _(mContext->mOutgoingMutex);
    mOutgoingBuffer.push_back(std::move(payload));
}

void ClientSocketManager::stop() {
    mContext->mStop.store(true);
}


void ClientSocketManager::loop() {
    using namespace std::chrono_literals;
    std::thread sender([&, ctx = mContext]{
       while (!mContext->mStop) {
           {
               std::lock(ctx->mOutgoingMutex, ctx->mSocketMutex);
               mutex_guard _1(ctx->mOutgoingMutex, std::adopt_lock);
               mutex_guard _2(ctx->mSocketMutex, std::adopt_lock);
               if (!mOutgoingBuffer.empty()) {
                   bool res = ctx->mSocket.send(mOutgoingBuffer.front());
                   (void) res;
                   mOutgoingBuffer.erase(mOutgoingBuffer.begin());
               }
           }
           std::this_thread::sleep_for(5ms);
       }
    });
    sender.detach();

    std::thread receiver([&, ctx = mContext]{
        while (!ctx->mStop) {
            {
                std::lock(ctx->mIncomingMutex, ctx->mSocketMutex);
                mutex_guard _1(ctx->mIncomingMutex, std::adopt_lock);
                mutex_guard _2(ctx->mSocketMutex, std::adopt_lock);
                auto res = ctx->mSocket.receive();
                if (res) mIncomingBuffer.push_back(*res);
            }
            std::this_thread::sleep_for(5ms);
        }
    });
    receiver.detach();
}