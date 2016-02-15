#pragma once

#include <core/string_view.hpp>
#include <vector>
#include <core/optional.hpp>
#include "sockets.h"

// I think it will be possible to get some common BaseSocket class out of both
// server and client. ~Kuba

class ClientSocket {
    static constexpr size_t receive_buffer_size = 1 << 16;
public:
    ClientSocket(unsigned short port, core::string_view address);
    ClientSocket(const int socketFd, const unsigned short port, core::string_view address);
    ClientSocket(ClientSocket&& other);
    ClientSocket& operator=(ClientSocket&& other);

    ClientSocket(const ClientSocket&) = delete;
    ClientSocket& operator=(const ClientSocket&) = delete;

    ~ClientSocket();

    bool isValid() const { return mMode == Mode::Initialized; }
    core::optional<size_t> send(const std::vector<char>& payload);
    core::optional<size_t> send(const void* payload, size_t size);
    core::optional<std::vector<char>> receive();
    
    int getSocketFd() const;

private:
    enum class Mode { Invalid, Initialized } mMode = Mode::Invalid;
    sockaddr_in mServer = {};
    int mSocketDesc = -1;
    std::vector<char> mReceiveBuffer = std::vector<char>(receive_buffer_size);
};
