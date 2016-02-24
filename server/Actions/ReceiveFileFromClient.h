#ifndef SERVER_RECEIVEFILEFROMCLIENT_H
#define SERVER_RECEIVEFILEFROMCLIENT_H

#include "common/api.h"
#include "common/client_api.h"
#include "common/communication_manager.h"
#include "IAction.h"

class ReceiveFileFromClient : public IAction{

public:

    int getType() const {
        return SendFileToServer::type;
    }

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);
};


#endif //SERVER_RECEIVEFILEFROMCLIENT_H
