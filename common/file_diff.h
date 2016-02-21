#pragma once

#include "common/file_scanner.h"

class FileDiff {
public:
    FileDiff(const std::vector<FileInfo>& mine, const std::vector<FileInfo>& other)
        : mMine(mine), mOther(other) {}

    DEFAULT_MOVE_COPY(FileDiff);

    std::vector<FileInfo> getAdded() const;
    std::vector<FileInfo> getModifiedOrAdded() const;
    std::vector<FileInfo> getDeleted() const;

private:
    std::vector<FileInfo> mMine;
    std::vector<FileInfo> mOther;
};