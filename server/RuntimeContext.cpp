#include "RuntimeContext.h"

RuntimeContext::RuntimeContext()
    :clientsManager(std::make_shared<ClientsManager>()),
     messageLoopState(ServerState::PENDING),
     state(ServerState::PENDING){

}

void RuntimeContext::close() {
    state = ServerState::CLOSED;
}

ServerState RuntimeContext::getState() const {
    return state;
}

void RuntimeContext::start() {
    if (state == ServerState::RUNNING){
        perror("Server is already running");
        return;
    }

    state = ServerState::RUNNING;
}
