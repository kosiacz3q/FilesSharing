#ifndef SERVER_SERVERSOCKET_H
#define SERVER_SERVERSOCKET_H

#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>

#include <core/optional.hpp>
#include "common/client_socket_manager.h"


class ServerSocket {

public:
    ServerSocket(const int port);

    ~ServerSocket();

    int getSocketFd() const;

private:

    enum class Mode { Invalid, Initialized } mMode = Mode::Invalid;
    int socketFd = -1;
};

typedef std::shared_ptr<ServerSocket> ServerSocketPtr;


#endif //SERVER_SERVERSOCKET_H
