#include <iostream>
#include "common/client_socket_manager.h"

using namespace std;

int main()
{
	ClientSocket sc(4096, "127.0.0.1");
//    sc.send("Yo!", 4);
//	cout << "Hello client\n";
//    auto res = sc.receive();
//    for (auto& x : *res) cout << x;
//    cout << "\n";

    ClientSockerManager manager(std::move(sc));
    sleep(1);
    manager.push({'Y', 'o', '!'});
    auto res = manager.pop();
    for (auto& x : res) cout << x;
        cout << "\n";
    sleep(1);
}