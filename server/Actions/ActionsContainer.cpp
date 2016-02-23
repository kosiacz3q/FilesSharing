#include "ActionsContainer.h"

ActionsContainer::ActionsContainer()
    :actions(ActionsMap()){

}

void ActionsContainer::registerAction(IActionPtr action) {

    actions.insert(std::make_pair(action->getType(), action));
    printf("Action for type %i registered\n", action->getType());
}

void ActionsContainer::resolve(std::unique_ptr<Api> request, CommunicationManagerPtr cm) {

    auto requestHandler = actions.find(request->getType());

    if (requestHandler == actions.end()){
        perror(("Request type " + std::to_string(request->getType()) + " is not supported").c_str());
        return;
    }

    requestHandler->second->handle(cm, std::move(request));
}
