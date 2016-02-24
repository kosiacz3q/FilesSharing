#ifndef SERVER_SENDFILELIST_H
#define SERVER_SENDFILELIST_H


#include "common/api.h"
#include "common/client_api.h"
#include "common/communication_manager.h"

#include "IAction.h"

class SendFileList : public IAction{

public:

    int getType() const {
        return GetFileList::type;
    }

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);

};


#endif //SERVER_SENDFILELIST_H
