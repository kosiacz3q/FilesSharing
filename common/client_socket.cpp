#include "client_socket.h"

#include <iostream>
#include <algorithm>

ClientSocket::ClientSocket(unsigned short port, const std::string& address) {
    mSocketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketDesc == -1) {
        std::cerr << "Could not create socket\n";
        perror("");
        return;
    }

    mServer.sin_addr.s_addr = inet_addr(address.c_str());
    mServer.sin_family = AF_INET;
    mServer.sin_port = htons(port);

    if (connect(mSocketDesc, (sockaddr*) &mServer, sizeof(mServer)) < 0)
    {
        std::cerr << "Clould not connect with " << address << ":" << port << "\n";
        perror("");
        return;
    }

    mMode = Mode::Initialized;
}

ClientSocket::~ClientSocket() {
    if (mMode == Mode::Invalid) return;
    close(mSocketDesc);
    mMode = Mode::Invalid;
    std::cerr << "Closed connection with " << mServer.sin_addr.s_addr << ":"
              << ntohs(mServer.sin_port) << "\n";
}

ClientSocket::ClientSocket(ClientSocket&& other) {
    std::swap(mServer, other.mServer);
    std::swap(mSocketDesc, other.mSocketDesc);
    std::swap(mMode, other.mMode);
}

ClientSocket& ClientSocket::operator=(ClientSocket&& other) {
    std::swap(mServer, other.mServer);
    std::swap(mSocketDesc, other.mSocketDesc);
    std::swap(mMode, other.mMode);
    return *this;
}


bool ClientSocket::send(const std::vector<char>& payload) {
    if (!isValid()) return false;

    if (::send(mSocketDesc, (const void*) payload.data(), payload.size(), 0) < 0) {
        perror("Could not sent");
        return false;
    }

    return  true;
}

