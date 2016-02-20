#ifndef SERVER_SENDTIMESTAMP_H
#define SERVER_SENDTIMESTAMP_H

#include <common/client_api.h>

#include "IAction.h"

class SendTimeStamp : public IAction{

public:

    int getType() const{
        return GetTime::type;
    };

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);

};


#endif //SERVER_SENDTIMESTAMP_H
