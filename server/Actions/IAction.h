#ifndef SERVER_IACTION_H
#define SERVER_IACTION_H

#include "common/communication_manager.h"


class IAction{

public:

    virtual int getType() const = 0;

    virtual void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg) = 0;

};

typedef std::shared_ptr<IAction> IActionPtr;

#endif //SERVER_IACTION_H
