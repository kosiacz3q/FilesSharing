#pragma once

#include "client_socket.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

class ClientSocketManager {
public:
    ClientSocketManager(ClientSocket&& socket);
    ~ClientSocketManager();

    bool isOutgoingBufferEmpty();
    bool isIncomingBufferEmpty();
    std::vector<char> pop();
    void push(std::vector<char> payload);
    void stop();

private:
    void loop();

    struct Context {
        Context(ClientSocket&& socket) : mSocket(std::move(socket)) {}

        std::vector<std::vector<char>> mIncomingBuffer;
        std::vector<std::vector<char>> mOutgoingBuffer;

        std::mutex mIncomingMutex;
        std::mutex mOutgoingMutex;
        std::mutex mSocketMutex;
        std::atomic<bool> mStop{false};

        ClientSocket mSocket;
    };
    std::shared_ptr<Context> mContext;
};
