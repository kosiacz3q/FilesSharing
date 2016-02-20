#include "file_scanner.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>

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
    res.push_back('\0');
    return res;
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