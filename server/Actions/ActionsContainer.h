#ifndef SERVER_ACTIONCONTAINER_H
#define SERVER_ACTIONCONTAINER_H

#include <map>
#include <memory>

#include "IAction.h"

class ActionsContainer {
public:

    ActionsContainer();

    void registerAction(IActionPtr action);

    void resolve(std::unique_ptr<Api> request, CommunicationManagerPtr cm);

private:

    typedef std::map<int, IActionPtr> ActionsMap;
    ActionsMap actions;

};

typedef std::shared_ptr<ActionsContainer> ActionsContainerPtr;

#endif //SERVER_ACTIONCONTAINER_H
