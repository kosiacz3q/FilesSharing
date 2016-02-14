#include <iostream>

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"

using namespace std;

int main()
{
	ClientSocket ss(4096, "127.0.0.1");

//    ClientSocketManager manager(std::move(ss));
//    manager.push({'Y', 'o', '!'});
//    sleep(5);
//    auto res = manager.pop();
//    for (auto& x : res) cout << x;
//    res = manager.pop();
//    for (auto& x : res) cout << x;

    // test with echo '22000A00000000000000' | xxd -r -p | nc -l -p 4096 > file.hex
    CommunicationManager cm(std::move(ss));
    GetTime gt(110);
    cm.send(gt);
    GetFileList gfl = cm.receive<GetFileList>(10);
    gfl.dump();
    sleep(3);
}