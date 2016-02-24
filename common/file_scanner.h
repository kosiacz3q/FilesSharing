#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <utility>
#include <type_traits>

#include "common/utils.h"

struct FileInfo {
    using TimeStampType = time_t;

    std::string path;
    TimeStampType timestamp;

    friend std::ostream& operator<<(std::ostream& os, const FileInfo& fileInfo) {
        return os << fileInfo.timestamp << "; " << fileInfo.path;
    }

    friend std::istream& operator>>(std::istream& os, FileInfo& fileInfo);

    bool operator==(const FileInfo& other) const {
        return timestamp == other.timestamp && path == other.path;
    }
};

class FileScanner {
public:
    FileScanner(const std::string& path);
    FileScanner(const std::vector<char> bytes);

    DEFAULT_MOVE_COPY(FileScanner);

    bool contains(const std::string& path) const;
    FileInfo getFileInfo(const std::string& path) const;

    static std::vector<char> getFileAsBytes(const std::string& path);
    static void saveBytesAsFile(const std::string& path, const std::vector<char>& bytes);
    static bool exists(const std::string& path);
    static void rename(const std::string& from, const std::string& to);
    static void remove(const std::string& path);
    static time_t getModificationTime(const std::string& path);
    static void setModificationTime(const std::string& path, FileInfo::TimeStampType time);
    static std::string joinPaths(const std::string& prefix, const std::string& sufix);

    std::vector<FileInfo> getDeletedSince(const FileScanner& previous) const;
    std::vector<FileInfo> getAddedSince(const FileScanner& previous) const;

    auto getPath() const { return mPath; }
    const auto& getFileList() const { return mFiles; }

    std::string asFileList() const;

    friend class DeletedFileList;

private:
    FileScanner() {}

    std::string mPath;
    std::vector<FileInfo> mFiles;
};