#include "client_logic.h"

#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <thread>

#include "common/file_scanner.h"
#include "common/client_api.h"
#include "common/server_api.h"

ClientLogic::ClientLogic(CommunicationManager& cm, const std::string& rootFolder)
        : mCM(cm)
        , mRoot(rootFolder) {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";
    std::cout << "Starting client. Root folder: " << mRoot << "\n";

    if (!checkTimeDiff()) return;

    std::cerr << "Starting loop.\n";
    auto res = loop();
    std::cerr << "Loop ended.\n";

    core::string_view errorMessage = "Closing without any errors.";
    switch (res) {
        case Error::TimeDiffError: errorMessage = "Time difference error."; break;
        case Error::Timeout: errorMessage = "Timeout. Communication aborted."; break;

        default: break;
    }

    std::cout << errorMessage << "\n";
}

bool ClientLogic::checkTimeDiff() {
    const auto now = time(nullptr);
    GetTime getTime(nextID());
    mCM.send(getTime);
    auto response = mCM.receiveBlocking<ServerTime>(currentID());
    if (!response) return false;

    auto serverTime = response->getTimestamp();
    std::cerr << "Current time:\t" << now << "\n";
    std::cerr << "Received time:\t" << serverTime << "\n";
    response->dump();
    response->dumpPayload();
    mTimeOffset = serverTime - now;
    if (std::abs(mTimeOffset) > 15) {
        std::cout << "Time difference is too big (" << mTimeOffset << "s).\n";
        return false;
    }

    return true;
}

ClientLogic::Error ClientLogic::loop() {
    using namespace std::chrono_literals;
    while (true) {
        FileScanner scanner(mRoot);

        GetFileList getFileList(nextID());
        mCM.send(getFileList);
        auto fileList = mCM.receiveBlocking<ServerFileList>(currentID());
        if (!fileList) return Error::Timeout;
        // TODO: handle incoming fileList

        std::this_thread::sleep_for(10s);
    }

    return Error::NoError;
}