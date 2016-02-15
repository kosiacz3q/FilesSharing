//
// Created by lucas on 2/15/16.
//

#include "ClientHandler.h"

ClientHandler::ClientHandler(CommunicationManagerPtr communicationManager, ActionsContainerPtr actionsContainer)
    : threadState(std::make_shared<AtomicThreadState>(ThreadState::PENDING)){

    id = communicationManager->getId();

    handlingThread = std::thread(
            &ClientHandler::handleLoop, this, threadState, communicationManager,actionsContainer);
}

ClientHandler::~ClientHandler() {

    {
        mutex_guard  _(stateMutex);
        *threadState = ThreadState::CLOSED;
    }

    handlingThread.join();
}

void ClientHandler::handleLoop(AtomicThreadStatePtr currentState,
                               CommunicationManagerPtr cm,
                               ActionsContainerPtr ac) {
    {
        mutex_guard  _(stateMutex);

        if (*currentState == ThreadState::CLOSED || *currentState == ThreadState::RUNNING)
            return;

        *currentState = ThreadState::RUNNING;
    }

    while(*currentState == ThreadState::RUNNING){

        auto currentMsg = cm->receiveNext();

        if (!currentMsg){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        ac->resolve(std::move(currentMsg), cm);
    }

    *currentState = ThreadState::CLOSED;
}
