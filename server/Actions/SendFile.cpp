#include "SendFile.h"

#include <common/server_api.h>
#include <common/utils.h>
#include <fstream>

void SendFile::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    FileFromServer ffs(msg->getID());

    //TODO refactor this ugly thing
    thread_local char pathToRequestedFile[50];

    from_bytes(
            msg->getPayload().begin(),
            msg->getPayload().end(),
            pathToRequestedFile);

    printf("Requested file %s\n", pathToRequestedFile);

    //TODO check if file exists

    perror("I don't give a #$@! what he wants, i will send my favorite file\n");

    thread_local std::ifstream fileToSend("syncRoot/testFile.file", std::ios_base::binary);

    ffs.setPayload(std::vector<char>(
            std::istreambuf_iterator<char>(fileToSend),
            std::istreambuf_iterator<char>()));

    ptr->send(ffs);

    printf("File sent");
}
