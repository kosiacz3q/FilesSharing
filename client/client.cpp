#include <iostream>

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"
#include "common/server_api.h"

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
    sleep(1);
    auto res = cm.receive<SendTime>(10);
    res.dump();
    std::cerr << "\n";
    for (auto& x : res.getPayload()) cerr << +x << " ";
    sleep(1);
}