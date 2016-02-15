#pragma once

#include <memory>
#include "../common/client_socket_manager.h"
#include "../common/api.h"

class CommunicationManager {
public:
    CommunicationManager(ClientSocket&& socket)
            : mSocketManager(std::move(socket)) {}

    void send(const Api& message);
    std::unique_ptr<Api> receive(uint32_t id);
    template<typename T>
    T receive(uint32_t id) {
        auto res = receive(id);
        assert(res);
        assert(res->getType() == T::type);
        return *unique_cast<T>(std::move(res));
    }

    int getId() const;

private:
    ClientSocketManager mSocketManager;
    std::vector<Api> mOutgoing;
    std::vector<std::unique_ptr<Api>> mIncoming;
};

typedef std::shared_ptr<CommunicationManager> CommunicationManagerPtr;