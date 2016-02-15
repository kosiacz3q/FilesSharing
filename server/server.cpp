#include <iostream>
#include "ServerManager.h"

using namespace std;

int main()
{
    constexpr int port = 4096;

    ServerManager ss = ServerManager(std::make_shared<ServerSocket>(port));

    ss.start();

    ss.close();

	cout << "Hello server\n";
}