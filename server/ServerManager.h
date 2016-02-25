#ifndef SERVER_SERVERMANAGER_H
#define SERVER_SERVERMANAGER_H

#include "common/client_socket_manager.h"
#include <sys/epoll.h>

#include "ServerSocket.h"
#include "RuntimeContext.h"

class ServerManager {
public:

    ServerManager(ServerSocketPtr serverSocket, ActionsContainerPtr actionsContainer);

    ~ServerManager();

    void start();

    void close();

private:

    bool registerEpoll();

    int pollFd;

    void connectingLoop(RuntimeContextPtr sCtx, const int serverSocketFd);

    RuntimeContextPtr context;
    ServerSocketPtr serverSocket;
    ActionsContainerPtr actionsContainer;
};


#endif //SERVER_SERVERMANAGER_H
