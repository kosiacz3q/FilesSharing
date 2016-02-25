#include "client_logic.h"

#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <thread>

#include <QFileSystemWatcher>

#include "common/file_diff.h"
#include "common/client_api.h"
#include "common/server_api.h"

ClientLogic::ClientLogic(CommunicationManager& cm, const std::string& rootFolder,
                         ConcurrentContext& cc)
        : mCM(cm)
        , mRoot(rootFolder)
        , mCC(cc) {
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
    auto* watcher = new QFileSystemWatcher();
    watcher->addPath("./");
    QObject::connect(watcher, &QFileSystemWatcher::directoryChanged, [] (const QString& str) {
        std::cerr << "Changed:\t" << str.toStdString() << std::endl;
    });
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
    bool first = true;
    while (true) {
        if (!first) {
            std::unique_lock<std::mutex> lk(mCC.m);
            mCC.isReady = false;
        }
        first = false;

        GetFileList getFileList(nextID());
        mCM.send(getFileList);
        auto fileList = mCM.receiveBlocking<ServerFileList>(currentID());
        if (!fileList) return Error::Timeout;
        auto& list = fileList->getFileList();
        FileScanner remoteFiles(::to_bytes_impl(list));
        auto res = onIncomingFileList(remoteFiles);//remoteFiles);
        if (res != Error::NoError) return res;

        {
            std::unique_lock<std::mutex> lk(mCC.m);
            while (!mCC.isReady)
            {
                mCC.cv.wait(lk);
                if (!mCC.isReady)
                    std::cerr << "Spurious wake up!\n";
            }
        }
        std::cerr << "File structure changed; next iteration!\n";
    }

    return Error::NoError;
}

ClientLogic::Error ClientLogic::onIncomingFileList(FileScanner remoteFiles) {
    FileScanner myFiles(mRoot);
    auto deletedByUser = mDeletedList.markAsDeleted(myFiles);
    std::cerr << "Deleted by user:\n";
    for (auto& x : deletedByUser) std::cerr << x << ", ";

    auto markedExistent = mDeletedList.markAsExistent(myFiles);
    std::cerr << "\nAdded previously deleted:\n";
    for (auto& x : markedExistent) std::cerr << x << ", ";
    std::cerr << "\n";

    mDeletedList.update(myFiles);
    // Delete remote files if I also deleted them:
    sendDeleteRequests(deletedByUser, remoteFiles);

    // Upload new files that were previously deleted and now are either not at the remote or newer.
    sendAddRequests(markedExistent, remoteFiles);

    GetDeletedList requestDeleted(nextID());
    mCM.send(requestDeleted);
    auto responseDeleted = mCM.receiveBlocking<ServerDeletedList>(currentID());
    if (!responseDeleted) return Error::Timeout;

    // Delete remotely deleted files
    std::vector<std::string> remotelyDeleted = responseDeleted->getDeletedList();
    {
       auto toDelete = mDeletedList.markAsDeleted(remotelyDeleted);
        for (auto& x: toDelete) {
            auto fullName = FileScanner::joinPaths(mRoot, x);
            if (FileScanner::exists(fullName)) {
                FileScanner::remove(fullName);
                std::cerr << "Removing remotely deleted file:\t" << x << "\n";
            }
        }
    }

    FileDiff diff(myFiles, remoteFiles);
    // Add remotely added/modified.
    std::vector<FileInfo> remotelyAddedOrModified = diff.getModifiedOrAdded();
    {
        std::vector<FileInfo> toDownload;
        for (auto& x : remotelyAddedOrModified) {
            if (std::find(deletedByUser.begin(), deletedByUser.end(), x.path)
                == deletedByUser.end())
                toDownload.push_back(x);
        }

        auto res = requestAndSaveNewFiles(toDownload);
        if (res != Error::NoError) return res;
        auto marked = mDeletedList.markAsExistent(extract(remotelyAddedOrModified,
                                            [] (auto& x) { return x.path; }));
        std::cerr << "Marked as deleted:\t";
        for (auto& x : marked) std::cerr << x << ", ";
        std::cerr << "\n";
    }

    diff = FileDiff(remoteFiles, myFiles);
    auto addedByMe = diff.getModifiedOrAdded();
    auto sendRes = sendAddRequests(extract(addedByMe, [](auto& x) { return x.path; }), remoteFiles);
    if (sendRes != Error::NoError) return sendRes;

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
    if (response->getStatus() != 0) return Error::NoError;

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

ClientLogic::Error ClientLogic::sendDeleteRequests(const std::vector<std::string>& toDelete,
                                                   const FileScanner& remote) {
    for (auto& x : toDelete)
        if (remote.contains(x)) {
            std::cerr << "Sending delete request:\t" << x << "\n";
            MarkAsDeleted del(nextID(), x);
            mCM.send(del);

            auto res = mCM.receiveBlocking<ServerDeletedResponse>(currentID());
            if (!res) return Error::Timeout;
        }
    return Error::NoError;
}

ClientLogic::Error ClientLogic::sendAddRequests(const std::vector<std::string>& toAdd,
                                                const FileScanner& remote) {
    for (auto& x : toAdd) {
        auto fullPath = FileScanner::joinPaths(mRoot, x);
        if (!FileScanner::exists(fullPath))
            continue;

        auto timestamp = FileScanner::getModificationTime(fullPath);
        if (!remote.contains(x) || remote.getFileInfo(x).timestamp < timestamp) {
            std::cerr << "Sending new file to server:\t" << x << "\n";
            SendFileToServer send(nextID(), x, mRoot, timestamp);
            mCM.send(send);

            auto res = mCM.receiveBlocking<FileFromClient>(currentID());
            if (!res) return Error::Timeout;
        }
    }
    return Error::NoError;
}
