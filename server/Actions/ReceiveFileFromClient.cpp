#include <common/server_api.h>

#include "ReceiveFileFromClient.h"
#include "common/file_scanner.h"
#include "common/deleted_file_list.h"

void ReceiveFileFromClient::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    FileFromClient ffc(msg->getID());

    auto uMsg = unique_cast<SendFileToServer>(std::move(msg));

    auto path = FileScanner::joinPaths("./syncRoot", uMsg->getPath());

    printf("Requested file [%s]\n", path.c_str());

    auto file = uMsg->getFile();

    FileScanner::saveBytesAsFile(path, file);
    FileScanner::setModificationTime(path, uMsg->getTimestamp());
    (void) DeletedListManager::getInstance().markAsExistent({path});

    ptr->send(ffc);

    printf("File received\n");
}
