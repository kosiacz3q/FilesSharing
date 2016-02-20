#include <iostream>

#include "common/file_scanner.h"

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"
#include "common/server_api.h"

using namespace std;

int main()
{
    for (auto it : recursive_directory_range("./test_dir")) {
        if (fs::is_regular_file(it.status())) std::cout << it << std::endl;
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

    switch(4){

        case 1: //timestamp
        {
            GetTime gt(110);
            cm.send(gt);
            printf("Request sent\n");
            auto res = cm.receiveBlocking<ServerTime>(110);
            res->dump();
            std::cerr << "\n";
            for (auto &x : res->getPayload()) cerr << +x << " ";
            cerr << "\n";
            break;
        }
        case 2: //file list
        {
            GetFileList gfl(120);
            cm.send(gfl);
            printf("Request sent\n");
            auto res = cm.receiveBlocking<ServerFileList>(120);
            res->dump();
            std::cerr << "\n";

            std::ofstream out("FileList", std::ios_base::binary);

            for (auto &x : res->getPayload()) out << (char)x;

            cerr << "\n";
            break;
        }
        case 3: //single file request
        {
            std::string fileName = "testFile.file";

            GetFileByPath gfl(150, fileName);

            cm.send(gfl);
            printf("Request sent\n");
            auto res = cm.receiveBlocking<FileFromServer>(150);
            res->dump();
            std::cerr << "\n" << std::hex;
            std::ofstream out(fileName, std::ios_base::binary);

            for (auto &x : res->getPayload()) {
                std::cerr << +x;
                out << x;
            }

            cerr << "\n";
            break;
        }
        case 4: //send single file
        {
            std::string fileName = "testFile2.file";

            std::ifstream fileToSend(fileName, std::ios_base::binary);

            SendFileToServer sft(160, fileName, std::vector<char>(
                    std::istreambuf_iterator<char>(fileToSend),
                    std::istreambuf_iterator<char>()));

            cm.send(sft);
            printf("Request sent\n");
            auto res = cm.receiveBlocking<FileFromClient>(160);
            res->dump();
            std::cerr << "\n";

            break;
        }

        default:
            break;
    }
}