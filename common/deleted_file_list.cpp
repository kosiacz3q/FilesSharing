#include "common/deleted_file_list.h"

#include <sys/types.h>
#include <sys/inotify.h>

static const auto& DeleteFileListName = ".deleted_filed";

DeletedFileList::DeletedFileList() {
    (void) DeletedListManager::getInstance(); // read list from file;
}

std::vector<std::string> DeletedFileList::getToMarkAsDeleted(FileScanner newest) {
    auto deletedCandidates = newest.getDeletedSince(mLast);
    std::vector<std::string> toMarkAsDeleted;

    for (auto& x : deletedCandidates) {
        if (DeletedListManager::getInstance().isMarkedAsDeleted(x.path))
            continue;
        toMarkAsDeleted.push_back(x.path);
    }

    DeletedListManager::getInstance().markAsDeleted(toMarkAsDeleted);
    mLast = std::move(newest);

    return toMarkAsDeleted;
}

void DeletedListManager::markAsDeleted(const std::vector<std::string>& toDeleted) {
    std::fstream file(DeleteFileListName, std::ios::ate);
    for (auto& x : toDeleted) {
        if (isMarkedAsDeleted(x))
            continue;
        mContent.push_back(x);
        file << x << "\n";
    }
}

void DeletedListManager::removeFromDeleted(const std::vector<std::string>& existent) {
    for (auto& x : existent) {
        auto it = std::find(mContent.begin(), mContent.end(), x);
        if (it != mContent.end())
            mContent.erase(it);
    }

    std::fstream file(DeleteFileListName, std::ios::trunc);
    for (auto& x : mContent)
        file << x << "\n";
}

bool DeletedListManager::isMarkedAsDeleted(const std::string& path) {
    return std::find(mContent.begin(), mContent.end(), path) != mContent.end();
}

DeletedListManager::DeletedListManager() {
    int fd = fd = inotify_init();
    rereadFile();
}

void DeletedListManager::rereadFile() {
    mContent.clear();
    std::fstream file(DeleteFileListName);
    std::string line;
    while (std::getline(file, line))
        if (!line.empty())
            mContent.push_back(line);
}