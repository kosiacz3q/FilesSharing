#include "client_socket_manager.h"

#include <cassert>
#include <chrono>

#include "utils.h"

ClientSockerManager::ClientSockerManager(ClientSocket&& socket)
    : mSocket(std::move(socket)) {
    assert(mSocket.isValid());
    std::thread t([&] { loop(); });
    t.detach();
}

ClientSockerManager::~ClientSockerManager() {
    mStop.store(false);
}

bool ClientSockerManager::isOutgoingBufferEmpty() {
    mutex_guard _(mOutgoingMutex);
    return mOutgoingBuffer.empty();
}

bool ClientSockerManager::isIncomingBufferEmpty() {
    mutex_guard _(mIncomingMutex);
    return mIncomingBuffer.empty();
}

std::vector<char> ClientSockerManager::pop() {
    mutex_guard _(mIncomingMutex);
    assert(!mIncomingBuffer.empty());
    auto res = mIncomingBuffer.front();
    mIncomingBuffer.erase(mIncomingBuffer.begin());
    return res;
}

void ClientSockerManager::push(std::vector<char> payload) {
    mutex_guard _(mOutgoingMutex);
    mOutgoingBuffer.push_back(std::move(payload));
}

void ClientSockerManager::loop() {
    while (!mStop) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5ms);
        {
            mutex_guard _(mOutgoingMutex);
            if (!mOutgoingBuffer.empty()) {
                bool res = mSocket.send(mOutgoingBuffer.front());
                (void) res;
                mOutgoingBuffer.erase(mOutgoingBuffer.begin());
            }
        }
        {
            mutex_guard _(mIncomingMutex);
            auto res = mSocket.receive();
            if (res) mIncomingBuffer.push_back(*res);
        }
    }
}