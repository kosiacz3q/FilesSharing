#include <iostream>
#include <cstring>
#include <thread>

#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QTimer>
#include <boost/filesystem.hpp>

#include "common/file_scanner.h"

#include "common/client_socket_manager.h"
#include "common/communication_manager.h"
#include "common/client_api.h"
#include "common/server_api.h"
#include "client_logic.h"

using namespace std;

void startQAppThread(int argc, char** argv, const char* path, ConcurrentContext& cc) {
    QCoreApplication app(argc, argv);
    std::cerr << "Hello Qt\n";

    QFileSystemWatcher watcher;
    auto absolutePath = boost::filesystem::absolute(path).string();
    std::cerr << "Absolute path:\t" << absolutePath << "\n";
    watcher.addPath(absolutePath.c_str());
    QTimer timer;
    timer.setInterval(5000);
    timer.setSingleShot(true);

    auto waker = [&] (auto&...) {
        std::cerr << "Hola amigo! Wakie wakie!" << std::endl;
        std::unique_lock<std::mutex> lk(cc.m);
        cc.isReady = true;
        cc.cv.notify_one();
        timer.stop();
        timer.start();
    };

    QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged, waker);
    QObject::connect(&timer, &QTimer::timeout, waker);
    timer.start();

    app.exec();
}

void startAppThread(int argc, char** argv, const char*& path, ConcurrentContext& cc) {
    std::thread t(startQAppThread, argc, argv, path, ref(cc));
    t.detach();
}

int main(int argc, char** argv)
{
    unsigned short port = 4096;
    const char* ip = "127.0.0.1";
    const char* rootFolder = "./test_dir";

    if (argc == 4) {
        ip = argv[1];
        sscanf(argv[2], "%hu", &port);
        rootFolder = argv[3];
    } else if (argc > 4) assert(false && "Too many arguments");

    ClientSocket ss(port, ip);
    if (ss.isValid() == false) {
        std::cout << "Could not connect.\nClient will now gracefully close.\n";
        return 1;
    }

    ConcurrentContext cc;
    startAppThread(argc, argv, rootFolder, cc);
    // test with echo '22000A00000000000000' | xxd -r -p | nc -l -p 4096 > file.hex

    CommunicationManager cm(std::move(ss));

    switch (0) {
        case 0: //normal use
        {
            ClientLogic _(cm, rootFolder, cc);

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

        }
            break;
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
        }
            break;
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
        }
            break;

        case 4: //send single file
        {
            std::string fileName = "testFile2.file";
            SendFileToServer sft(160, fileName, "", time(nullptr));

            cm.send(sft);
            printf("Request sent\n");

            auto res = cm.receiveBlocking<FileFromClient>(160);
            printf("Response received\n");

            if (res)
                res->dump();
            else
                perror("Null response");

            std::cerr << "\n";

        }
            break;

        case 5: //remove request
        {
            std::string fileName = "testFile2.file";
            MarkAsDeleted mas(230, fileName);

            cm.send(mas);
            printf("Request sent\n");

            auto res = cm.receiveBlocking<ServerDeletedResponse>(230);
            printf("Response received\n");

            if (res)
                res->dump();
            else
                perror("Null response");

            std::cerr << "\n";
        }
            break;

        case 6: //get deleted list
        {
            GetDeletedList gdl(599);

            cm.send(gdl);
            printf("Request sent\n");

            auto res = cm.receiveBlocking<ServerDeletedList>(599);



            printf("Response received\n");

            if (res) {

                for (auto toRemove : res->getDeletedList())
                std::cerr << toRemove << "\n";
            }
            else
                perror("Null response");
        }
            break;

        default:
            break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(8));
    std::cout << "Client's gracefully stopped." << std::endl;
}