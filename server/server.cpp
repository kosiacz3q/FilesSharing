#include <boost/algorithm/string.hpp>

#include <iostream>
#include <server/Actions/SendFileList.h>
#include <server/Actions/SendFile.h>
#include <server/Actions/ReceiveFileFromClient.h>
#include <server/Actions/MarkAsDeletedAction.h>
#include <server/Actions/GetServerDeletedList.h>
#include "ServerManager.h"

#include "Actions/SendTimeStamp.h"

using namespace std;

void checkForExitCommand();

int main()
{
    constexpr int port = 4096;

    auto ac = std::make_shared<ActionsContainer>();

    ac->registerAction(std::make_shared<SendTimeStamp>());
    ac->registerAction(std::make_shared<SendFileList>());
    ac->registerAction(std::make_shared<SendFile>());
    ac->registerAction(std::make_shared<ReceiveFileFromClient>());
    ac->registerAction(std::make_shared<MarkAsDeletedAction>());
    ac->registerAction(std::make_shared<GetServerDeletedList>());

    ServerManager ss = ServerManager(std::make_shared<ServerSocket>(port), ac);

    ss.start();

    checkForExitCommand();

    ss.close();

    cout << "Rest in peace.\n";
}


void checkForExitCommand(){

    auto exitCommand = std::set<std::string> {
            "q", "quit", "exit"
    };

    std::string t;

    do {
        std::cin >> t;
        boost::algorithm::to_lower(t);

        printf("ew\n");

    }while(exitCommand.find(t) == exitCommand.end());
}