#include "ServerSocket.h"

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

    mMode = Mode::Initialized;
}

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
}

ServerSocket::~ServerSocket() {

    if (mMode != Mode::Initialized)
        return;

    close(socketFd);

}
