#include "file_scanner.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>

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