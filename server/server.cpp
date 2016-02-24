#include <boost/algorithm/string.hpp>

#include <iostream>
#include "server/Actions/SendFileList.h"
#include "server/Actions/SendFile.h"
#include "server/Actions/ReceiveFileFromClient.h"
#include "server/Actions/MarkAsDeletedAction.h"
#include "server/Actions/GetServerDeletedList.h"
#include "ServerManager.h"

#include "Actions/SendTimeStamp.h"

#include <boost/filesystem.hpp>

using namespace std;

void checkForExitCommand();

int main(int argc, char** argv)
{
    unsigned short port = 4096;
    if (argc == 2) {
        sscanf(argv[1], "%hu", &port);
    }
    else if (argc >=2) assert(false && "Too many arguments");

    boost::filesystem::create_directory("syncRoot");

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
    } while (exitCommand.find(t) == exitCommand.end());
}