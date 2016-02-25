#ifndef SERVER_CLIENTSMANAGER_H
#define SERVER_CLIENTSMANAGER_H

#include <memory>
#include <set>

#include "common/communication_manager.h"
#include "ClientHandler.h"

class ClientsManager {

public:

    ClientsManager();

    void AddClient(ClientHandlerPtr newClient);

    void removeClient(const int clientId);

    void disconectAll();

    void cleanup();

private:

    std::mutex exlusiveClientsListAccess;
    std::set<ClientHandlerPtr, bool (*)(const ClientHandlerPtr&, const ClientHandlerPtr&)> clients;
};

typedef std::shared_ptr<ClientsManager> ClientsManagerPtr;

#endif //SERVER_CLIENTSMANAGER_H
