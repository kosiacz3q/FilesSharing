#include <thread>

#include "ServerManager.h"

ServerManager::ServerManager(ServerSocketPtr serverSocket,
                             ActionsContainerPtr actionsContainer)
    :context(std::make_shared<RuntimeContext>()),
     serverSocket(serverSocket),
     actionsContainer(actionsContainer){

}

ServerManager::~ServerManager() {

}

void ServerManager::start() {

    if (context->getState() == ServerState::RUNNING)
    {
        perror("Server is already running");
        return;
    }

    context->start();

    std::thread st(&ServerManager::connectingLoop, this, context, serverSocket->getSocketFd());

    st.detach();
}

void ServerManager::close() {
    context->close();

    while (context->messageLoopState == ServerState::RUNNING)
        std::this_thread::sleep_for(std::chrono::microseconds(1000));

    context->clientsManager->disconectAll();
}

void ServerManager::connectingLoop(RuntimeContextPtr sCtx, const int serverSocketFd) {

    sCtx->messageLoopState = ServerState::RUNNING;

    printf("Accepting connections\n");
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    sockaddr in_addr;
    socklen_t in_len = sizeof(in_addr);

    int cleanupTime = 200;


    while (sCtx->getState() == ServerState::RUNNING) {

        int infd;

        while((infd = accept (serverSocketFd, &in_addr, &in_len)) != -1)
        {
            if (getnameinfo (&in_addr, in_len, hbuf, sizeof hbuf, sbuf, sizeof sbuf,
                             NI_NUMERICHOST | NI_NUMERICSERV) == 0){
                printf("Accepted connection on descriptor %d ""(host=%s, port=%s)\n", infd, hbuf, sbuf);

                ClientSocket ss(infd, std::stoi(sbuf), hbuf);

                sCtx->clientsManager->AddClient(
                        std::make_shared<ClientHandler>(
                               std::make_shared<CommunicationManager>(std::move(ss)), actionsContainer));
            }

            if (!--cleanupTime){
                sCtx->clientsManager->cleanup();
                cleanupTime = 200;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    sCtx->messageLoopState = ServerState::CLOSED;
    printf("Message loop closed\n");
}

