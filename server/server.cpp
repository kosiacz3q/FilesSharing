#include <iostream>
#include "ServerManager.h"

using namespace std;

int main()
{
    constexpr int port = 4096;

    ServerManager ss = ServerManager(std::make_shared<ServerSocket>(port));

    ss.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    ss.close();

	cout << "Hello server\n";
}