#include <common/server_api.h>
#include "MarkAsDeletedAction.h"

#include <boost/filesystem.hpp>

#include <common/deleted_file_list.h>

void MarkAsDeletedAction::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    auto sdr = ServerDeletedResponse(msg->getID());

    auto uMsg = unique_cast<MarkAsDeleted>(std::move(msg));

    auto fullPath = "./syncRoot/" + uMsg->getPath();

    printf("Remove File [%s] request\n", fullPath.c_str());

    DeletedListManager::getInstance().markAsDeleted(uMsg->getPath());
    if(!FileScanner::exists(fullPath)){
        printf("File [%s] does not exists.", fullPath.c_str());
        sdr.setStatus(1);
        ptr->send(sdr);
        return;
    }

    FileScanner::remove(fullPath);

    ptr->send(sdr);

    printf("File [%s] removed\n", fullPath.c_str());
}

