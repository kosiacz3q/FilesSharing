#include "SendFile.h"

#include <common/server_api.h>
#include <common/utils.h>
#include "common/file_scanner.h"

void SendFile::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    FileFromServer ffs(msg->getID());

    //TODO refactor this ugly thing
    // when spawning a new thread with a function ref, local variables are (thread) local... ~Kuba
    thread_local char pathToRequestedFile[50];

    from_bytes(
            msg->getPayload().begin(),
            msg->getPayload().end(),
            pathToRequestedFile);

    printf("Requested file %s\n", pathToRequestedFile);

    //TODO check if file exists

    perror("I don't give a #$@! what he wants, i will send my favorite file\n");

    ffs.setPayload(std::vector<char>(FileScanner::getFileAsBytes("syncRoot/testFile.file")));

    ptr->send(ffs);

    printf("File sent");
}
