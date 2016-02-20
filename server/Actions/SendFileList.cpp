#include <common/server_api.h>
#include "SendFileList.h"

#include <fstream>

void SendFileList::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    ServerFileList sfl(msg->getID());

    std::ifstream fileList("FileList", std::ios_base::binary);

    assert(fileList.is_open());

    auto payload = std::vector<char>(
            std::istreambuf_iterator<char>(fileList),
            std::istreambuf_iterator<char>());

    sfl.setPayload(payload);

    ptr->send(sfl);

    printf("FileList sent\n");
}
