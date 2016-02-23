#include <common/server_api.h>
#include "MarkAsDeletedAction.h"

#include <boost/filesystem.hpp>

#include <common/deleted_file_list.h>

void MarkAsDeletedAction::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    auto sdr = ServerDeletedResponse(msg->getID());

    auto uMsg = unique_cast<MarkAsDeleted>(std::move(msg));

    auto path = "syncRoot/" + uMsg->getPath();

    printf("Remove File [%s] request\n", path.c_str());

    if(!FileScanner::exists(path)){
        printf("File [%s] does not exists.", path.c_str());
        sdr.setStatus(1);
        ptr->send(sdr);
        return;
    }

    DeletedListManager::getInstance().markAsDeleted(path);

    FileScanner::remove(path);

    ptr->send(sdr);

    printf("File [%s] removed\n", path.c_str());
}

