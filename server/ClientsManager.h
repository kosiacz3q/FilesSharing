#ifndef SERVER_CLIENTSMANAGER_H
#define SERVER_CLIENTSMANAGER_H

#include <memory>
#include <set>

#include <common/client_socket_manager.h>

class ClientsManager {

public:

    ClientsManager();

    void AddClient(const int socketFd);

    void removeClient(const int clientId);

    void disconectAll();

private:

    std::mutex exlusiveClientsListAccess;
    std::set<ClientSocketManager, bool (*)(const ClientSocketManager&, const ClientSocketManager&)> clients;
};

typedef std::shared_ptr<ClientsManager> ClientsManagerPtr;

#endif //SERVER_CLIENTSMANAGER_H
