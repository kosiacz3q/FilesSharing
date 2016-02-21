#include "SendTimeStamp.h"

#include "common/server_api.h"

void SendTimeStamp::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    ServerTime st(msg->getID());

    ptr->send(st);
    printf("Timestamp sent\n");
}
