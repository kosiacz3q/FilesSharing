#pragma once

#include "client_socket.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class ClientSockerManager {
public:
    ClientSockerManager(ClientSocket&& socket);
    ~ClientSockerManager();

    bool isOutgoingBufferEmpty();
    bool isIncomingBufferEmpty();
    std::vector<char> pop();
    void push(std::vector<char> payload);

private:

    void loop();

    std::vector<std::vector<char>> mIncomingBuffer;
    std::vector<std::vector<char>> mOutgoingBuffer;
    std::mutex mIncomingMutex;
    std::mutex mOutgoingMutex;
    std::atomic<bool> mStop{false};
    ClientSocket mSocket;
};