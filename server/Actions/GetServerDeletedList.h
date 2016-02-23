#ifndef SERVER_GETSERVERDELETEDLIST_H
#define SERVER_GETSERVERDELETEDLIST_H

#include <common/client_api.h>

#include "IAction.h"

class GetServerDeletedList : public IAction{
public:

    int getType() const {
        return GetDeletedList::type;
    }

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);
};


#endif //SERVER_GETSERVERDELETEDLIST_H
