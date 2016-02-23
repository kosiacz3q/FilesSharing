#include "SendFile.h"

#include "common/server_api.h"
#include "common/utils.h"
#include "common/file_scanner.h"

void SendFile::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    std::string pathToRequestedFile(msg->getPayload().data());

    printf("Requested file %s\n", pathToRequestedFile.c_str());

    if(!FileScanner::exists(pathToRequestedFile)){
        printf("File [%s] does not exists.", pathToRequestedFile.c_str());
        ptr->send(FileFromServer(msg->getID(), 1));
        return;
    }
    
    FileFromServer ffs(msg->getID(), pathToRequestedFile, "./syncRoot");
    ptr->send(ffs);

    printf("File sent");
}
