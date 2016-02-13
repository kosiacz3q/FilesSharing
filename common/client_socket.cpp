#include "client_socket.h"

#include <iostream>
#include <fcntl.h>

static bool SetSocketBlockingEnabled(int fd, bool blocking)
{
    if (fd < 0) return false;

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;
    flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return fcntl(fd, F_SETFL, flags) == 0;
}


ClientSocket::ClientSocket(unsigned short port, core::string_view address) {
    mSocketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketDesc == -1) {
        std::cerr << "Could not create socket\n";
        perror("");
        return;
    }

    mServer.sin_addr.s_addr = inet_addr(address.data());
    mServer.sin_family = AF_INET;
    mServer.sin_port = htons(port);

    if (connect(mSocketDesc, (sockaddr*) &mServer, sizeof(mServer)) < 0)
    {
        std::cerr << "Clould not connect with " << address << ":" << port << "\n";
        perror("");
        return;
    }

    if (!SetSocketBlockingEnabled(mSocketDesc, false)) {
        perror("Could not set non-blocking");
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

bool ClientSocket::send(const void* payload, size_t size) {
    if (!isValid()) return false;

    if (::send(mSocketDesc, payload, size, 0) < 0) {
        perror("Could not sent");
        return false;
    }

    return  true;
}

bool ClientSocket::send(const std::vector<char>& payload) {
    return send(payload.data(), payload.size());
}

core::optional<std::vector<char>> ClientSocket::receive() {
    if (!isValid()) return core::nullopt;

    auto res = recv(mSocketDesc, mReceiveBuffer.data(), mReceiveBuffer.size(), 0);
    if (res < 0) {
        return core::nullopt;
    }

    return std::vector<char>(mReceiveBuffer.begin(), mReceiveBuffer.begin() + res);
}


