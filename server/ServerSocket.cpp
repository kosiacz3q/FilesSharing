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

    if (listen(socketFd, SOMAXCONN) == -1){
        perror("Could not start listening on socket");
        exit(-32);
    }

    mMode = Mode::Initialized;
}

ServerSocket::~ServerSocket() {

    if (mMode != Mode::Initialized) {
        printf("Server socket not initialized");
        return;
    }

    printf("Closing server socket\n");
    close(socketFd);
}

int ServerSocket::getSocketFd() const {
    return socketFd;
}
