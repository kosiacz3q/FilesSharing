#include <common/server_api.h>
#include <fstream>
#include "ReceiveFileFromClient.h"

void ReceiveFileFromClient::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    FileFromClient ffc(msg->getID());

    auto uMsg = unique_cast<SendFileToServer>(std::move(msg));

    printf("Requested file %s\n", uMsg->getPath().c_str());

    //TODO register file change


    assert(receivedFile.is_open());

    thread_local std::vector<char>::const_iterator fileBegin, fileEnd;

    uMsg->getFile(fileBegin, fileEnd);

    thread_local std::ofstream receivedFile("syncRoot/" + uMsg->getPath(), std::ios_base::binary);

    while (fileBegin != fileEnd){

        receivedFile << *fileBegin;
        ++fileBegin;
    }

    ptr->send(ffc);

    printf("File received");
}
