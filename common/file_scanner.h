#pragma once

#include <boost/filesystem.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <utility>
#include <type_traits>

namespace fs = boost::filesystem;

struct recursive_directory_range {
    using iterator = fs::recursive_directory_iterator;
    recursive_directory_range(fs::path p) : p_(p) {}

    iterator begin() { return fs::recursive_directory_iterator(p_); }
    iterator end() { return fs::recursive_directory_iterator(); }

    fs::path p_;
};

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
    static_assert(std::is_same<FileInfo::TimeStampType, decltype(time(0))>(), "");
    static_assert(std::is_same<FileInfo::TimeStampType, decltype(fs::last_write_time({}))>(), "");

    FileScanner(const std::string& path);
    FileScanner(const std::vector<char> bytes);
    FileScanner(const FileScanner&) = default;
    FileScanner& operator=(const FileScanner&) = default;
    FileScanner(FileScanner&&) = default;
    FileScanner& operator=(FileScanner&&) = default;
    static std::vector<char> getFileAsBytes(const std::string& path);
    static void saveBytesAsFile(const std::string& path, const std::vector<char>& bytes);
    static bool exists(const std::string& path);

    auto getPath() const { return mPath; }
    const auto& getFileInfo() const { return mFiles; }

    std::string asFileList() const;

private:
    std::string mPath;
    std::vector<FileInfo> mFiles;
};