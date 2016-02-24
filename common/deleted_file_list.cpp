#include "common/deleted_file_list.h"

#include <iostream>
#include <fstream>

static const char* getDeletedListName() {
    return ".deleted_files_list";
}

DeletedFileList::DeletedFileList() {
    (void) DeletedListManager::getInstance(); // read list from file;
}

std::vector<std::string> DeletedFileList::markAsDeleted(const std::vector<std::string>& toDelete) {
    std::vector<std::string> toMarkAsDeleted;

    for (auto& x : toDelete) {
        if (DeletedListManager::getInstance().isMarkedAsDeleted(x))
            continue;
        toMarkAsDeleted.push_back(x);
    }

    DeletedListManager::getInstance().markAsDeleted(toMarkAsDeleted);

    return toMarkAsDeleted;
}

std::vector<std::string> DeletedFileList::markAsDeleted(FileScanner newest) {
    auto deletedCandidates = newest.getDeletedSince(mLast);
    return markAsDeleted(extract(deletedCandidates, [](auto& x) { return x.path; }));
}

std::vector<std::string> DeletedFileList::markAsExistent(const std::vector<std::string>& toAdd) {
    std::vector<std::string> toMarkAsAdded;
    for (auto& x : toAdd) {
        if (DeletedListManager::getInstance().isMarkedAsDeleted(x))
            toMarkAsAdded.push_back(x);
    }

    DeletedListManager::getInstance().markAsExistent(toMarkAsAdded);

    return toMarkAsAdded;
}

std::vector<std::string> DeletedFileList::markAsExistent(FileScanner newest) {
    auto addedCandidates = newest.getAddedSince(mLast);
    return markAsExistent(extract(addedCandidates, [](auto& x) { return x.path; }));
}

void DeletedListManager::markAsDeleted(const std::vector<std::string>& toDeleted) {
    std::ofstream file(getDeletedListName(), std::ios::app);
    assert(file.is_open() && file.good() && "Cannot open file");

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

    FileScanner::remove(getDeletedListName());
    std::ofstream file(getDeletedListName(),  std::ios::app);
    assert(file.is_open());
    assert(file.good());
    for (auto& x : mContent) {
        file << x << "\n";
        std::cerr << "Restored file from memorized deleted:\t" << x << "\n";
    }
}

bool DeletedListManager::isMarkedAsDeleted(const std::string& path) {
    return std::find(mContent.begin(), mContent.end(), path) != mContent.end();
}

DeletedListManager::DeletedListManager() {
    rereadFile();
}

void DeletedListManager::rereadFile() {
    mContent.clear();
    std::cerr << "Rereading " << getDeletedListName() << "\n";

    { std::ofstream _(getDeletedListName(), std::ios::app); } // Create file if not existent.
    std::ifstream file(getDeletedListName());
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) {
            mContent.push_back(line);
            std::cerr << "Remembered deleted file:\t" << line << "\n";
        }
}