#include "file_scanner.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include <cassert>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static_assert(std::is_same<FileInfo::TimeStampType, decltype(time(nullptr))>(), "");
static_assert(std::is_same<FileInfo::TimeStampType, decltype(fs::last_write_time({}))>(), "");

struct recursive_directory_range {
    using iterator = fs::recursive_directory_iterator;
    recursive_directory_range(fs::path p) : mP(p) {}

    iterator begin() { return iterator{mP}; }
    iterator end() { return {}; }

    fs::path mP;
};

static fs::path relativeTo(fs::path from, fs::path to) {
    // Start at the root path and while they are the same then do nothing then when they first
    // diverge take the remainder of the two path and replace the entire from path with ".."
    // segments.
    auto fromIter = from.begin();
    auto toIter = to.begin();

    // Loop through both
    while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter)) {
        ++toIter;
        ++fromIter;
    }

    fs::path finalPath;
    while (fromIter != from.end()) {
        finalPath /= "..";
        ++fromIter;
    }

    while (toIter != to.end()) {
        finalPath /= *toIter;
        ++toIter;
    }

    return finalPath;
}

std::istream& operator>>(std::istream& os, FileInfo& fileInfo) {
    os >> fileInfo.timestamp;
    char sc;
    os >> sc;
    char space;
    os.get(space);
    char buffer[4096] = {};
    os.getline(buffer, sizeof(buffer));
    fileInfo.path = std::string(buffer);

    return os;
}

FileScanner::FileScanner(const std::string& path)
    : mPath(path) {
    if (!fs::exists(path)) {
        fs::create_directory(path);
    }
    assert(fs::is_directory(path));
    for (auto it : recursive_directory_range(path)) {
        if (fs::is_regular_file(it.status())) {
            std::cerr << it << " " << fs::last_write_time(it.path()) << std::endl;
            auto relative = relativeTo(fs::path(path), it.path());
            mFiles.push_back({relative.string(), fs::last_write_time(it.path())});
        }
    }
}

FileScanner::FileScanner(const std::vector<char> bytes) {
    std::string str(bytes.data(), bytes.data() + bytes.size());
    std::istringstream iss(str);

    FileInfo fi;
    while (iss >> fi)
        mFiles.push_back(fi);
}

std::vector<char> FileScanner::getFileAsBytes(const std::string& path) {
    assert(exists(path));

    std::ifstream file(path, std::ios::binary);
    std::vector<char> res;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(),
              std::back_inserter(res));
    return res;
}

void FileScanner::saveBytesAsFile(const std::string& path, const std::vector<char>& bytes) {
    fs::create_directories(fs::path(path).parent_path());
    std::ofstream file(path, std::ios::binary);
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(file));
}

bool FileScanner::exists(const std::string& path) {
    return fs::exists(path) && fs::is_regular_file(path);
}

std::string FileScanner::asFileList() const {
    std::ostringstream oss;
    for (auto& file : mFiles) {
        oss << file << "\n";
        std::cerr << "File: " << file << "\n";
    }

    return oss.str();
}

void FileScanner::rename(const std::string& from, const std::string& to) {
    assert(exists(from));
    assert(!exists(to));
    fs::rename(from, to);
}

void FileScanner::remove(const std::string& path) {
    assert(fs::exists(path)); // can be something else then a regular file
    fs::remove(path);
}

time_t FileScanner::getModificationTime(const std::string& path) {
    assert(exists(path));
    return fs::last_write_time(path);
}

void FileScanner::setModificationTime(const std::string& path, FileInfo::TimeStampType time) {
    assert(exists(path));
    fs::last_write_time(path, time);
}

std::string FileScanner::joinPaths(const std::string& prefix, const std::string& sufix) {
    return (fs::path(prefix) / fs::path(sufix)).string();
}

std::vector<FileInfo> FileScanner::getDeletedSince(const FileScanner& previous) const {
    const auto& previousFiles = previous.mFiles;
    std::vector<FileInfo> deleted;

    for (auto& file : previousFiles) {
        if (std::find_if(mFiles.begin(), mFiles.end(), [&](auto& x) { return x.path == file.path; })
                == mFiles.end()) {
            deleted.push_back(file);
        }
    }

    return deleted;
}

std::vector<FileInfo> FileScanner::getAddedSince(const FileScanner& previous) const {
    const auto& previousFiles = previous.mFiles;
    std::vector<FileInfo> added;

    for (auto& file : mFiles) {
        if (std::find_if(previousFiles.begin(), previousFiles.end(),
                         [&](auto& x) { return x.path == file.path; })
            == previousFiles.end()) {
            added.push_back(file);
        }
    }

    return added;
}

bool FileScanner::contains(const std::string& path) const {
    for (auto& x : mFiles)
        if (x.path == path)
            return true;
    return false;
}

FileInfo FileScanner::getFileInfo(const std::string& path) const {
    for (auto& x : mFiles)
        if (x.path == path)
            return x;
    assert(false);
    return {};
}
