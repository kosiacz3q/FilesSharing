#ifndef SERVER_CLIENTSMANAGER_H
#define SERVER_CLIENTSMANAGER_H

#include <memory>
#include <set>

#include "common/communication_manager.h"

class ClientsManager {

public:

    ClientsManager();

    void AddClient(CommunicationManagerPtr newClient);

    void removeClient(const int clientId);

    void disconectAll();

private:

    std::mutex exlusiveClientsListAccess;
    std::set<CommunicationManagerPtr, bool (*)(const CommunicationManagerPtr&, const CommunicationManagerPtr&)> clients;
};

typedef std::shared_ptr<ClientsManager> ClientsManagerPtr;

#endif //SERVER_CLIENTSMANAGER_H
