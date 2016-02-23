#ifndef SERVER_MARKASDELETED_H
#define SERVER_MARKASDELETED_H

#include <common/client_api.h>


#include "IAction.h"

class MarkAsDeletedAction : public IAction{

public:

    int getType() const {
        return MarkAsDeleted::type;
    }

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);
};


#endif //SERVER_MARKASDELETED_H
