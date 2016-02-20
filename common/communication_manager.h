#pragma once

#include <memory>
#include <chrono>
#include "common/client_socket_manager.h"
#include "common/api.h"

class CommunicationManager {
public:
    CommunicationManager(ClientSocket&& socket)
            : mSocketManager(std::move(socket)) {}

    void send(const Api& message);
    std::unique_ptr<Api> receive(uint32_t id);
    template<typename T>
    core::optional<T> receive(uint32_t id) {
        auto res = receive(id);
        if (!res) return core::nullopt;
        assert(res->getType() == T::type);
        return *unique_cast<T>(std::move(res));
    }

    std::unique_ptr<Api> receiveBlocking(uint32_t id,
                             std::chrono::milliseconds timeout = std::chrono::milliseconds(3000));
    template<typename T>
    core::optional<T> receiveBlocking(uint32_t id,
                          std::chrono::milliseconds timeout = std::chrono::milliseconds(3000)) {
        auto res = receiveBlocking(id, timeout);
        if (!res) return core::nullopt;
        assert(res->getType() == T::type);
        return *unique_cast<T>(std::move(res));
    }

    std::unique_ptr<Api> receiveNext();

    int getId() const;

private:

    void refreshMessages();

    ClientSocketManager mSocketManager;
    std::vector<Api> mOutgoing;
    std::vector<std::unique_ptr<Api>> mIncoming;

};

typedef std::shared_ptr<CommunicationManager> CommunicationManagerPtr;