#include <iostream>
#include "common/client_socket_manager.h"

using namespace std;

int main()
{
	ClientSocket ss(4096, "127.0.0.1");
//    sc.send("Yo!", 4);
//	cout << "Hello client\n";
//    auto res = sc.receive();
//    for (auto& x : *res) cout << x;
//    cout << "\n";

    ClientSocketManager manager(std::move(ss));
    manager.push({'Y', 'o', '!'});
    sleep(5);
    auto res = manager.pop();
    for (auto& x : res) cout << x;
    res = manager.pop();
    for (auto& x : res) cout << x;
}