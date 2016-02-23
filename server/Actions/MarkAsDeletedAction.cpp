#include <common/server_api.h>
#include "MarkAsDeletedAction.h"

#include <boost/filesystem.hpp>

#include <common/deleted_file_list.h>

void MarkAsDeletedAction::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    auto sdr = ServerDeletedResponse(msg->getID());

    auto uMsg = unique_cast<MarkAsDeleted>(std::move(msg));

    auto path = "syncRoot/" + uMsg->getPath();

    printf("Remove File [%s] request\n", path.c_str());

    DeletedListManager::getInstance().markAsDeleted(path);

    FileScanner::remove(path);

    ptr->send(sdr);

    printf("File [%s] removed\n", path.c_str());
}

