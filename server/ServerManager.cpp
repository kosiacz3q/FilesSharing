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

    if (!registerEpoll()){
        perror("Cannot register epoll events");
        exit(53);
    }

    context->start();

    std::thread st(&ServerManager::connectingLoop, this, context, pollFd, serverSocket->getSocketFd());

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

    context->clientsManager->disconectAll();
}

void ServerManager::connectingLoop(RuntimeContextPtr sCtx, const int pollFd, const int serverSocketFd) {

    const int maxPendingEventsCount = 64;

    thread_local int countOfEvents;
    thread_local epoll_event* events = new epoll_event[maxPendingEventsCount];

    sCtx->messageLoopState = ServerState::RUNNING;

    while (sCtx->getState() == ServerState::RUNNING) {

        constexpr int timeoutInMilliseconds = 1000;
        countOfEvents = epoll_wait(pollFd, events, maxPendingEventsCount, timeoutInMilliseconds);

        //TODO maybe somehow replacing this with foreach will be nicer
        for (int i = 0; i < countOfEvents; ++i) {

            printf("new event\n");

            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {

                perror("Epoll error on one of sockets\n");
                sCtx->clientsManager->removeClient(events[i].data.fd);

                continue;
            }
            else if (events[i].data.fd == serverSocketFd)
            {
                printf("Accepting new connection\n");
                thread_local sockaddr in_addr;
                thread_local socklen_t in_len = sizeof(in_addr);

                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

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
                };
            }else{
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    delete[] events;

    sCtx->messageLoopState = ServerState::CLOSED;
    printf("Message loop closed\n");

}

