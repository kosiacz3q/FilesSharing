#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <linux/inotify.h>

#include "common/file_scanner.h"

class DeletedFileList {
public:
    DeletedFileList();
    std::vector<std::string> getToMarkAsDeleted(FileScanner newest);

private:
    FileScanner mLast;
};

class DeletedListManager {
public:
    static DeletedListManager& getInstance() {
        static DeletedListManager dfm;
        return dfm;
    }

    void markAsDeleted(const std::string& toDeleted){
        markAsDeleted({toDeleted});
    };
    void markAsDeleted(const std::vector<std::string>& toDeleted);
    void removeFromDeleted(const std::vector<std::string>& existent);
    bool isMarkedAsDeleted(const std::string& path);

private:
    DeletedListManager();
    void rereadFile();

    std::vector<std::string> mContent;
};