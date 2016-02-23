#include "common/deleted_file_list.h"

#include <iostream>

static const auto& DeletedFileListName = ".deleted_files";

DeletedFileList::DeletedFileList() {
    (void) DeletedListManager::getInstance(); // read list from file;
}

std::vector<std::string> DeletedFileList::markAsDeleted(FileScanner newest) {
    auto deletedCandidates = newest.getDeletedSince(mLast);
    std::vector<std::string> toMarkAsDeleted;

    for (auto& x : deletedCandidates) {
        if (DeletedListManager::getInstance().isMarkedAsDeleted(x.path))
            continue;
        toMarkAsDeleted.push_back(x.path);
    }

    DeletedListManager::getInstance().markAsDeleted(toMarkAsDeleted);

    return toMarkAsDeleted;
}

std::vector<std::string> DeletedFileList::markAsExistent(FileScanner newest) {
    auto addedCandidates = newest.getAddedSince(mLast);
    std::vector<std::string> toMarkAsAdded;

    for (auto& x : addedCandidates) {
        if (DeletedListManager::getInstance().isMarkedAsDeleted(x.path))
            toMarkAsAdded.push_back(x.path);
    }

    DeletedListManager::getInstance().markAsExistent(toMarkAsAdded);

    return toMarkAsAdded;
}

void DeletedListManager::markAsDeleted(const std::vector<std::string>& toDeleted) {
    std::ofstream file(DeletedFileListName, std::ios::app);
    assert(file.is_open() && "Cannot open file");

    for (auto& x : toDeleted) {
        if (isMarkedAsDeleted(x))
            continue;
        mContent.push_back(x);
        file << x << "\n";
    }
}

void DeletedListManager::markAsExistent(const std::vector<std::string>& existent) {
    for (auto& x : existent) {
        auto it = std::find(mContent.begin(), mContent.end(), x);
        std::cerr << "Erasing:\t" << x << "\n";
        if (it != mContent.end())
            mContent.erase(it);
    }

    FileScanner::remove(DeletedFileListName);
    std::fstream file(DeletedFileListName, std::ios::trunc);
    assert(file.is_open());
    assert(file.good());
    for (auto& x : mContent)
        file << x << "\n";
}

bool DeletedListManager::isMarkedAsDeleted(const std::string& path) {
    return std::find(mContent.begin(), mContent.end(), path) != mContent.end();
}

DeletedListManager::DeletedListManager() {
    rereadFile();
}

void DeletedListManager::rereadFile() {
    mContent.clear();
    std::cerr << "Rereading " << DeletedFileListName << "\n";

    std::fstream file(DeletedFileListName, std::ios::trunc);
    if (!FileScanner::exists(DeletedFileListName)) std::cerr << "File does not exits\n";
    std::string line;
    while (std::getline(file, line))
        if (!line.empty())
            mContent.push_back(line);
}