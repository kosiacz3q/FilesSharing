#include <iostream>
#include <server/Actions/SendFileList.h>
#include "ServerManager.h"

#include "Actions/SendTimeStamp.h"

using namespace std;

int main()
{
    constexpr int port = 4096;

    auto ac = std::make_shared<ActionsContainer>();

    ac->registerAction(std::make_shared<SendTimeStamp>());
    ac->registerAction(std::make_shared<SendFileList>());

    ServerManager ss = ServerManager(std::make_shared<ServerSocket>(port), ac);

    ss.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    ss.close();

	cout << "Rest in peace.\n";
}