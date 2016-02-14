#include <thread>
#include <sys/epoll.h>

#include "ServerManager.h"


ServerManager::ServerManager(ServerSocketPtr serverSocket)
    :context(std::make_shared<RuntimeContext>()),
     serverSocket(serverSocket){

}

void ServerManager::start() {

    if (context->getState() == ServerState::RUNNING)
    {
        perror("Server is already running");
        return;
    }

    registerEpool();

    serverSocket->GetNextClient();

/*
    std::thread st([sCtx = context]{

        while (sCtx->getState() == ServerState::RUNNING) {

            if ()

        }
    });

    st.detach();*/
}

void ServerManager::registerEpool() {

    //epoll_create()

}

void ServerManager::close() {
    context->close();
}
