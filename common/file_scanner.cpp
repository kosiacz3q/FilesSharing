#include "file_scanner.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static_assert(std::is_same<FileInfo::TimeStampType, decltype(time(0))>(), "");
static_assert(std::is_same<FileInfo::TimeStampType, decltype(fs::last_write_time({}))>(), "");

struct recursive_directory_range {
    using iterator = fs::recursive_directory_iterator;
    recursive_directory_range(fs::path p) : p_(p) {}

    iterator begin() { return fs::recursive_directory_iterator(p_); }
    iterator end() { return fs::recursive_directory_iterator(); }

    fs::path p_;
};

std::istream& operator>>(std::istream& os, FileInfo& fileInfo) {
    os >> fileInfo.timestamp;
    char sc;
    os >> sc;
    assert(sc == ';');
    char space;
    os.get(space);
    assert(space = ' ');
    char buffer[4096] = {};
    os.getline(buffer, sizeof(buffer));
    fileInfo.path = std::string(buffer);

    return os;
}

FileScanner::FileScanner(const std::string& path)
    : mPath(path) {
    for (auto it : recursive_directory_range(path)) {
        if (fs::is_regular_file(it.status())) {
            std::cerr << it << " " << fs::last_write_time(it.path()) << std::endl;
            mFiles.push_back({it.path().string(), fs::last_write_time(it.path())});
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
    assert(exists(path));
    fs::remove(path);
}