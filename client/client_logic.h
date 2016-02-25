#pragma once

#include <string>
#include <cstdint>
#include "common/file_scanner.h"
#include "common/deleted_file_list.h"

#include "common/communication_manager.h"

class ClientLogic {
public:
    ClientLogic(CommunicationManager& cm, const std::string& rootFolder, ConcurrentContext& cc);

    enum class Error {
        NoError,
        TimeDiffError,
        Timeout,
    };

private:
    bool checkTimeDiff();

    uint32_t nextID() { return ++mMessageID; }
    uint32_t currentID() { return mMessageID; }

    Error loop();
    Error onIncomingFileList(FileScanner remoteFiles);
    Error deleteFiles(const std::vector<FileInfo>& toDelete);
    Error requestAndSaveNewFiles(const std::vector<FileInfo>& toAdd);
    Error requestAndSaveNewFile(FileInfo file, const std::string& fullPath);

    Error sendDeleteRequests(const std::vector<std::string>& toDelete, const FileScanner& remote);
    Error sendAddRequests(const std::vector<std::string>& toAdd, const FileScanner& remote);

    CommunicationManager& mCM;
    DeletedFileList mDeletedList;
    std::string mRoot;
    uint32_t mMessageID = 0;
    time_t mTimeOffset = 0;
    ConcurrentContext& mCC;
};

