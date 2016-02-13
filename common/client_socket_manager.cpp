#include "client_socket_manager.h"

#include <cassert>
#include <chrono>

#include "utils.h"

ClientSocketManager::ClientSocketManager(ClientSocket&& socket)
    : mSocket(std::move(socket)) {
    assert(mSocket.isValid());
    loop();
}

ClientSocketManager::~ClientSocketManager() {
    stop();
}

bool ClientSocketManager::isOutgoingBufferEmpty() {
    mutex_guard _(mOutgoingMutex);
    return mOutgoingBuffer.empty();
}

bool ClientSocketManager::isIncomingBufferEmpty() {
    mutex_guard _(mIncomingMutex);
    return mIncomingBuffer.empty();
}

std::vector<char> ClientSocketManager::pop() {
    mutex_guard _(mIncomingMutex);
    assert(!mIncomingBuffer.empty());
    auto res = mIncomingBuffer.front();
    mIncomingBuffer.erase(mIncomingBuffer.begin());
    return res;
}

void ClientSocketManager::push(std::vector<char> payload) {
    mutex_guard _(mOutgoingMutex);
    mOutgoingBuffer.push_back(std::move(payload));
}

void ClientSocketManager::stop() {
    mStop.store(true);
}


void ClientSocketManager::loop() {
    using namespace std::chrono_literals;
    std::thread sender([&]{
       while (!mStop) {
           {
               std::lock(mOutgoingMutex, mSocketMutex);
               mutex_guard _1(mOutgoingMutex, std::adopt_lock);
               mutex_guard _2(mSocketMutex, std::adopt_lock);
               if (!mOutgoingBuffer.empty()) {
                   bool res = mSocket.send(mOutgoingBuffer.front());
                   (void) res;
                   mOutgoingBuffer.erase(mOutgoingBuffer.begin());
               }
           }
           std::this_thread::sleep_for(5ms);
       }
    });
    sender.detach();

    std::thread receiver([&]{
        while (!mStop) {
            {
                std::lock(mIncomingMutex, mSocketMutex);
                mutex_guard _1(mIncomingMutex, std::adopt_lock);
                mutex_guard _2(mSocketMutex, std::adopt_lock);
                auto res = mSocket.receive();
                if (res) mIncomingBuffer.push_back(*res);
            }
            std::this_thread::sleep_for(5ms);
        }
    });
    receiver.detach();
}