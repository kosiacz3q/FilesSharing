#ifndef SERVER_SENDTIMESTAMP_H
#define SERVER_SENDTIMESTAMP_H

#include <common/client_api.h>

#include "IAction.h"

class SendTimeStamp : public IAction{

    int getType() const;

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);

};


#endif //SERVER_SENDTIMESTAMP_H
