#include <iostream>
#include <cstring>

#include "common/file_scanner.h"

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"
#include "common/server_api.h"
#include "client_logic.h"

using namespace std;

int main(int argc, char** argv)
{
    unsigned short port = 4096;
    const char* ip = "127.0.0.1";
    const char* rootFolder = "./test_dir";

    if (argc == 4) {
        sscanf(argv[1], "%hu", &port);
        ip = argv[2];
        rootFolder = argv[3];
    } else if (argc > 4) assert(false && "Too many arguments");

    ClientSocket ss(port, ip);
    if (ss.isValid() == false) {
        std::cout << "Could not connect.\nClient will now gracefully close.\n";
        return 1;
    }

    // test with echo '22000A00000000000000' | xxd -r -p | nc -l -p 4096 > file.hex

    CommunicationManager cm(std::move(ss));

    switch (0) {
        case 0: //normal use
        {
            ClientLogic _(cm, rootFolder);

            break;
        }
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