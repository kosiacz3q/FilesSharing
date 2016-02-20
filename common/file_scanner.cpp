#include "file_scanner.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

FileScanner::FileScanner(core::string_view path)
    : mPath(path) {
    for (auto it : recursive_directory_range(path.to_string())) {
        if (fs::is_regular_file(it.status())) {
            std::cerr << it << " " << fs::last_write_time(it.path()) << std::endl;
            mFiles.emplace_back(it.path().string(), fs::last_write_time(it.path()));
        }
    }
}

std::vector<char> FileScanner::getFileAsBytes(const std::string& path) {
    assert(fs::is_regular_file(path));

    std::ifstream file(path, std::ios::binary);
    std::vector<char> res;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(),
              std::back_inserter(res));
    return res;
}