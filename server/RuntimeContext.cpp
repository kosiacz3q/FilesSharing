//
// Created by lucas on 2/14/16.
//

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
