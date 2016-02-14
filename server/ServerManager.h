#ifndef SERVER_SERVERMANAGER_H
#define SERVER_SERVERMANAGER_H

#include <common/client_socket_manager.h>

#include "ServerSocket.h"
#include "RuntimeContext.h"

class ServerManager {
public:

    ServerManager(ServerSocketPtr serverSocket);

    void start();

    void close();

private:

    void registerEpool();

    RuntimeContextPtr context;
    ServerSocketPtr serverSocket;
};


#endif //SERVER_SERVERMANAGER_H
