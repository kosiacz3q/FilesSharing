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
    int getClientId() const;

private:
    void loop();
    core::optional<std::vector<char>> getFullIncomingMessage();

    struct Context {
        Context(ClientSocket&& socket) : mSocket(std::move(socket)) {}

        std::vector<char> mIncomingBuffer;
        std::vector<std::vector<char>> mOutgoingBuffer;

        std::mutex mIncomingMutex;
        std::mutex mOutgoingMutex;
        std::atomic<bool> mStop{false};

        ClientSocket mSocket;
    };
    std::shared_ptr<Context> mContext;
};

using ClientSocketManagerPtr = std::shared_ptr<ClientSocketManager>;
