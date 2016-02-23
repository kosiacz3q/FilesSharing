#include <common/server_api.h>
#include "MarkAsDeletedAction.h"

#include <boost/filesystem.hpp>

#include <common/deleted_file_list.h>


void MarkAsDeletedAction::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    auto sdr = ServerDeletedResponse(msg->getID());

    auto uMsg = unique_cast<MarkAsDeleted>(std::move(msg));

    printf("Remove File [%s] request\n", uMsg->getPath().c_str());

    DeletedListManager::getInstance().markAsDeleted(uMsg->getPath());

    FileScanner::remove(uMsg->getPath());

    ptr->send(sdr);

    printf("File [%s] removed\n", uMsg->getPath().c_str());
}

