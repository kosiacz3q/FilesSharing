#pragma once

#include <string>
#include <vector>
#include <core/optional.hpp>
#include "sockets.h"

// I think it will be possible to get some common BaseSocket class out of both
// server and client. ~Kuba

class ClientSocket {
public:
    ClientSocket(unsigned short port, const std::string& address);
    ClientSocket(ClientSocket&& other);
    ClientSocket& operator=(ClientSocket&& other);

    ClientSocket(const ClientSocket&) = delete;
    ClientSocket& operator=(const ClientSocket&) = delete;

    ~ClientSocket();

    bool isValid() const { return mMode == Mode::Initialized; }
    bool send(const std::vector<char>& payload);
    core::optional<std::vector<char>> receive();

private:
    enum class Mode { Invalid, Initialized } mMode = Mode::Invalid;
    sockaddr_in mServer = {};
    int mSocketDesc = -1;
};
