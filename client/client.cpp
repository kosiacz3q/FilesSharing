#include <iostream>
#include "common/client_socket.h"

using namespace std;

int main()
{
	ClientSocket sc(4096, "127.0.0.1");
    sc.send("Yo!", 4);
	cout << "Hello client\n";
    auto res = sc.receive();
    for (auto& x : *res) cout << x;
    cout << "\n";
}