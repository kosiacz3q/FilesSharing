//
// Created by lucas on 2/14/16.
//

#ifndef SERVER_RUNTIMECONTEXT_H
#define SERVER_RUNTIMECONTEXT_H

#include<memory>
#include<mutex>
#include <atomic>

#include "ClientsManager.h"

enum class ServerState
{
    PENDING,
    RUNNING,
    CLOSED
};

class RuntimeContext {

public:

    RuntimeContext();

    ServerState getState () const;

    void close();

    ClientsManagerPtr clientsManager;

private:

    std::atomic<ServerState> state;
};

typedef std::shared_ptr<RuntimeContext> RuntimeContextPtr;

#endif //SERVER_RUNTIMECONTEXT_H
