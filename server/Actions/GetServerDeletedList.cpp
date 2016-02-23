#include "GetServerDeletedList.h"

#include <common/deleted_file_list.h>
#include <common/server_api.h>

void GetServerDeletedList::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg){

    printf("Get deleted list request received\n");

    auto sdl = ServerDeletedList(msg->getID(), DeletedListManager::getInstance().getMarkedAsDeleted());

    ptr->send(sdl);

    printf("Response sent\n");
}