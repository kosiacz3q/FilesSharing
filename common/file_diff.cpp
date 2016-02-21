#include "common/file_diff.h"

FileDiff::FileDiff(const std::vector<FileInfo>& mine, const std::vector<FileInfo>& other) {
    for (auto& file : mine)
        mMine.emplace(file.path, file.timestamp);
    for (auto& file : other)
        mOther.emplace(file.path, file.timestamp);
}

std::vector<FileInfo> FileDiff::getAdded() const {
    std::vector<FileInfo> res;
    for (auto& otherFile : mOther)
        if (mMine.count(otherFile.first) == 0)
            res.push_back({otherFile.first, otherFile.second});
    return res;
}

std::vector<FileInfo> FileDiff::getModifiedOrAdded() const {
    std::vector<FileInfo> res;
    for (auto& otherFile : mOther)
        if (mMine.count(otherFile.first) == 0
            || mMine.find(otherFile.first)->second < otherFile.second)
            res.push_back({otherFile.first, otherFile.second});
    return res;
}

std::vector<FileInfo> FileDiff::getDeleted() const {
    std::vector<FileInfo> res;
    for (auto& myFile : mMine)
        if (mOther.count(myFile.first) == 0)
            res.push_back({myFile.first, myFile.second});
    return res;
}
