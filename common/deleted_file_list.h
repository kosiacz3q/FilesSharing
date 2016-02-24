#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

#include "common/file_scanner.h"

class DeletedFileList {
public:
    DeletedFileList();
    std::vector<std::string> markAsDeleted(const std::vector<std::string>& toDelete);
    std::vector<std::string> markAsDeleted(FileScanner newest);
    std::vector<std::string> markAsExistent(FileScanner newest);
    void update(FileScanner newest) { mLast = std::move(newest); }

private:
    FileScanner mLast;
};

class DeletedListManager {
public:
    static DeletedListManager& getInstance() {
        static DeletedListManager dfm;
        return dfm;
    }

    void markAsDeleted(const std::string& toDeleted) {
        markAsDeleted( std::vector<std::string>{toDeleted});
    };
    void markAsDeleted(const std::vector<std::string>& toDeleted);
    void markAsExistent(const std::vector<std::string>& existent);
    bool isMarkedAsDeleted(const std::string& path);
    const auto& getMarkedAsDeleted() const { return mContent; }

private:
    DeletedListManager();
    void rereadFile();

    std::vector<std::string> mContent;
};