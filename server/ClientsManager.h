#ifndef SERVER_CLIENTSMANAGER_H
#define SERVER_CLIENTSMANAGER_H

#include <list>
#include <memory>

#include <common/client_socket_manager.h>

class ClientsManager {

public:

    ClientsManager();

    void AddClient();


private:

    std::list<ClientSocketManager> clients;
};

typedef std::shared_ptr<ClientsManager> ClientsManagerPtr;

#endif //SERVER_CLIENTSMANAGER_H
