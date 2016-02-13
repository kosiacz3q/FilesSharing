#include <iostream>
#include "common/client_socket_manager.h"

using namespace std;

int main()
{
	ClientSocket ss(4096, "127.0.0.1");

    ClientSocketManager manager(std::move(ss));
    manager.push({'Y', 'o', '!'});
    sleep(5);
    auto res = manager.pop();
    for (auto& x : res) cout << x;
    res = manager.pop();
    for (auto& x : res) cout << x;
}