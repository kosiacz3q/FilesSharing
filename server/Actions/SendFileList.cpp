#include <common/server_api.h>
#include "SendFileList.h"

#include <fstream>

void SendFileList::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    ServerFileList sfl(msg->getID());

    thread_local std::ifstream fileList("syncRoot/FileList", std::ios_base::binary);

    assert(fileList.is_open());

    sfl.setPayload(std::vector<char>(
            std::istreambuf_iterator<char>(fileList),
            std::istreambuf_iterator<char>()));

    ptr->send(sfl);

    printf("FileList sent\n");
}
