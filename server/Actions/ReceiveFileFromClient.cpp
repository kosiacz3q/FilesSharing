#include <common/server_api.h>
#include <fstream>

#include "ReceiveFileFromClient.h"
#include "common/file_scanner.h"

void ReceiveFileFromClient::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    FileFromClient ffc(msg->getID());

    auto uMsg = unique_cast<SendFileToServer>(std::move(msg));

    printf("Requested file %s\n", uMsg->getPath().c_str());

    //TODO register file change
    assert(receivedFile.is_open());

    auto file = uMsg->getFile();
    auto fullName = FileScanner::joinPaths("./syncRoot", uMsg->getPath());
    FileScanner::saveBytesAsFile(fullName, file);
    FileScanner::setModificationTime(fullName, uMsg->getTimestamp());

    printf("File saved\n");

    ptr->send(ffc);

    printf("File received\n");
}
