#pragma once

#include <unordered_map>

#include "common/file_scanner.h"

class FileDiff {
public:
    // What the other has changed in relation to mine.
    FileDiff(const std::vector<FileInfo>& mine, const std::vector<FileInfo>& other);
    FileDiff(const FileScanner& mine, const FileScanner& other)
            : FileDiff(mine.getFileList(), other.getFileList()) {}

    DEFAULT_MOVE_COPY(FileDiff);

    std::vector<FileInfo> getAdded() const;
    std::vector<FileInfo> getModifiedOrAdded() const; // Return newer timestamp.
    std::vector<FileInfo> getDeleted() const;

private:
    std::unordered_map<std::string, FileInfo::TimeStampType> mMine;
    std::unordered_map<std::string, FileInfo::TimeStampType> mOther;
};