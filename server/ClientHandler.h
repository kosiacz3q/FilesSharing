#ifndef SERVER_CLIENTHANDLER_H
#define SERVER_CLIENTHANDLER_H


#include <common/communication_manager.h>
#include <server/Actions/ActionsContainer.h>
#include "Actions/IAction.h"

class ClientHandler {
public:

    ClientHandler(CommunicationManagerPtr communicationManager,
                  ActionsContainerPtr actionsContainer);
    ~ClientHandler();

    int getId() const {
        return id;
    }

private:

    enum class ThreadState { PENDING, RUNNING, CLOSED};

    typedef std::atomic<ThreadState> AtomicThreadState;
    typedef std::shared_ptr<AtomicThreadState> AtomicThreadStatePtr;

    void handleLoop(AtomicThreadStatePtr currentState, CommunicationManagerPtr cm, ActionsContainerPtr ac);

    AtomicThreadStatePtr threadState;

    std::mutex stateMutex;

    std::thread handlingThread;

    int id;
};

typedef std::shared_ptr<ClientHandler> ClientHandlerPtr;

#endif //SERVER_CLIENTHANDLER_H
