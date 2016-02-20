#include "file_scanner.h"

#include <iostream>

FileScanner::FileScanner(core::string_view path)
    : mPath(path) {
    for (auto it : recursive_directory_range("./test_dir")) {
        if (fs::is_regular_file(it.status())) {
            std::cerr << it << " " << fs::last_write_time(it.path()) << std::endl;
            mFiles.emplace_back(it.path().string(), fs::last_write_time(it.path()));
        }
    }
}
