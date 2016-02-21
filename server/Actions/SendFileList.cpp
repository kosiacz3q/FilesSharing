#include "SendFileList.h"

#include "common/server_api.h"

void SendFileList::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    ServerFileList sfl(msg->getID(), "./syncRoot");
    ptr->send(sfl);

    printf("FileList sent\n");
}
