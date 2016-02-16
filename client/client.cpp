#include <iostream>

#include <boost/filesystem.hpp>

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"
#include "common/server_api.h"

using namespace std;
namespace fs = boost::filesystem;

struct recursive_directory_range {
    using iterator = fs::recursive_directory_iterator;
    recursive_directory_range(fs::path p) : p_(p) {}

    iterator begin() { return fs::recursive_directory_iterator(p_); }
    iterator end() { return fs::recursive_directory_iterator(); }

    fs::path p_;
};


int main()
{
    for (auto it : recursive_directory_range(".")) {
        std::cout << it << std::endl;
    }

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