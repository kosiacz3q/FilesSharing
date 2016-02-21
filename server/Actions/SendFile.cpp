#include "SendFile.h"

#include "common/server_api.h"
#include "common/utils.h"
#include "common/file_scanner.h"

void SendFile::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {
    //TODO refactor this ugly thing
    // when spawning a new thread with a function ref, local variables are (thread) local... ~Kuba
    std::string pathToRequestedFile(msg->getPayload().data());

    printf("Requested file %s\n", pathToRequestedFile.c_str());

    //TODO check if file exists
    assert(FileScanner::exists(pathToRequestedFile) && "Handle me!");

    perror("I don't give a #$@! what he wants, i will send my favorite file\n");

    FileFromServer ffs(msg->getID(), "testFile.file", "./syncRoot");
    ptr->send(ffs);

    printf("File sent");
}
