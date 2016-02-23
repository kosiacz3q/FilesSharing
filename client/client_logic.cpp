#include "client_logic.h"

#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <thread>

#include "common/file_diff.h"
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
        auto& list = fileList->getFileList();
        FileScanner remoteFiles(::to_bytes_impl(list));
        auto res = onIncomingFileList(remoteFiles);
        if (res != Error::NoError) return res;

        std::this_thread::sleep_for(10s);
    }

    return Error::NoError;
}

ClientLogic::Error ClientLogic::onIncomingFileList(FileScanner remoteFiles) {
    //TODO: finish handling deleted files using DeletedFileList amd DeletedListManager
    FileScanner myFiles(mRoot);
    auto deletedByUser = mDeletedList.markAsDeleted(myFiles);
    std::cerr << "Deleted by user:\n";
    for (auto& x : deletedByUser) std::cerr << x << ", ";

    auto markedExistent = mDeletedList.markAsExistent(myFiles);
    std::cerr << "\nAdded previously deleted:\n";
    for (auto& x : markedExistent) std::cerr << x << ", ";
    std::cerr << "\n";

    mDeletedList.update(myFiles);

    FileDiff diff(myFiles, remoteFiles);

    GetDeletedList requestDeleted(nextID());
    mCM.send(requestDeleted);
    auto responseDeleted = mCM.receiveBlocking<ServerDeletedList>(currentID());
    if (!responseDeleted) return Error::Timeout;

    const std::vector<std::string>& remotelyDeleted = responseDeleted->getDeletedList();
    std::vector<std::string> toDelete;
    for (auto& x : deletedByUser) {
        if (std::find(remotelyDeleted.begin(), remotelyDeleted.end(), x)
                != remotelyDeleted.end()) {
            toDelete.push_back(x);
        }
    }
    auto deleteRes = deleteFiles(extract(toDelete, [](auto& x) { return FileInfo{x, 0}; }));
    if (deleteRes != Error::NoError) return deleteRes;

    auto toAdd = diff.getModifiedOrAdded();

    auto requestRes = requestAndSaveNewFiles(toAdd);
    if (requestRes != Error::NoError) return requestRes;

    return Error::NoError;
}

ClientLogic::Error ClientLogic::deleteFiles(const std::vector<FileInfo>& toDelete) {
    std::cerr << "Deleting files:\n";
    for (auto& x : toDelete) {

        auto fullName = FileScanner::joinPaths(mRoot, x.path);
        std::cerr << fullName << "\n";
        assert(FileScanner::exists(fullName));
        FileScanner::remove(fullName);
    }

    return Error::NoError;
}

ClientLogic::Error ClientLogic::requestAndSaveNewFile(FileInfo file, const std::string& fullPath) {
    GetFileByPath get(nextID(), file.path);
    mCM.send(get);
    auto response = mCM.receiveBlocking<FileFromServer>(currentID());
    if (!response) return Error::Timeout;

    const auto& fileAsBytes = response->getFile();
    FileScanner::saveBytesAsFile(fullPath, fileAsBytes);
    FileScanner::setModificationTime(fullPath, file.timestamp);

    return Error::NoError;
}

ClientLogic::Error ClientLogic::requestAndSaveNewFiles(const std::vector<FileInfo>& toAdd) {
    std::cerr << "Adding or replacing files:\n";
    for (auto& x : toAdd) {
        auto fullName = FileScanner::joinPaths(mRoot, x.path);
        std::cerr << fullName << "\n";
        if (FileScanner::exists(fullName)) {
            FileScanner::remove(fullName);
            std::cerr << "Deleting:\t" << fullName << "\n";
        }
        auto res = requestAndSaveNewFile(x, fullName);
        if (res != Error::NoError) return res;
    }

    return Error::NoError;
}

