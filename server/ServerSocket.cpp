#include "ServerSocket.h"

static bool SetSocketBlockingEnabled(int fd, bool blocking) {
    if (fd < 0) return false;

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return fcntl(fd, F_SETFL, flags) == 0;
}

ServerSocket::ServerSocket(const int port) {

    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in mServer = {};

    mServer.sin_addr.s_addr =  INADDR_ANY;
    mServer.sin_family = AF_INET;
    mServer.sin_port = htons(port);

    if (bind(socketFd, (struct sockaddr *) &mServer, sizeof(mServer)) < 0) {
        perror("ERROR on binding");
        return;
    }

    if (!SetSocketBlockingEnabled(socketFd, false)) {
        perror("Could not set non-blocking");
        return;
    }

    mMode = Mode::Initialized;
}
/*
core::optional<ClientSocketManager> ServerSocket::GetNextClient() {

    constexpr int maxNumberOfPendingConnections = 100;
    listen(socketFd, maxNumberOfPendingConnections);

    sockaddr_in newClientAdd;
    socklen_t len = sizeof(sockaddr_in);
    if (auto f = accept(socketFd, (sockaddr*)&newClientAdd, &len) > 0){
        perror("Client connected successfully");
        close(f);
    };

    return core::optional<ClientSocketManager>();
}*/

ServerSocket::~ServerSocket() {

    if (mMode != Mode::Initialized)
        return;

    close(socketFd);

}

int ServerSocket::getSocketFd() const {
    return socketFd;
}
