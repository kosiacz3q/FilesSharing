#include <iostream>
#include "common/client_socket.h"

using namespace std;

int main()
{
	ClientSocket sc(4096, "127.0.0.1");
    sc.send({'Y', 'o', '!'});
	cout << "Hello client\n";
}