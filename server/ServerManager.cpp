#include <thread>
#include <chrono>

#include "ServerManager.h"


ServerManager::ServerManager(ServerSocketPtr serverSocket)
    :context(std::make_shared<RuntimeContext>()),
     serverSocket(serverSocket){

}

ServerManager::~ServerManager() {

}

void ServerManager::start() {

    if (context->getState() == ServerState::RUNNING)
    {
        perror("Server is already running");
        return;
    }

    if (!registerEpoll()){
        perror("Cannot register epoll events");
        exit(53);
    }

    std::thread st(&ServerManager::messageLoop, this, context, pollFd, serverSocket->getSocketFd());

    st.detach();
}

bool ServerManager::registerEpoll() {

    if ((pollFd = epoll_create1(0)) == -1){
        perror("Epoll cannot be created");
        return false;
    }

    epoll_event event;
    event.data.fd = serverSocket->getSocketFd();
    event.events = EPOLLIN | EPOLLET;

    if (epoll_ctl(pollFd, EPOLL_CTL_ADD, serverSocket->getSocketFd(), &event) == -1)
    {
        perror("Cannot configure epoll");
        return false;
    }

    return true;
}

void ServerManager::close() {
    context->close();

    while (context->messageLoopState == ServerState::RUNNING)
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

void ServerManager::messageLoop(RuntimeContextPtr sCtx, const int pollFd, const int serverSocketFd) {

    const int maxPendingEventsCount = 64;

    thread_local int countOfEvents;
    epoll_event* events = new epoll_event[maxPendingEventsCount];

    sCtx->messageLoopState = ServerState::RUNNING;

    while (sCtx->getState() == ServerState::RUNNING) {

        constexpr int timeoutInMilliseconds = 1000;
        countOfEvents = epoll_wait(pollFd, events, maxPendingEventsCount, timeoutInMilliseconds);

        //TODO maybe somehow replacing this with foreach will be nicer
        for (int i = 0; i < countOfEvents; ++i) {

            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {

                perror("Epoll error on one of sockets");
                sCtx->clientsManager->removeClient(events[i].data.fd);

                continue;
            }
            else if (events[i].data.fd == serverSocketFd)
            {

            }
        }
    }

    delete[] events;

    sCtx->messageLoopState = ServerState::CLOSED;
}

