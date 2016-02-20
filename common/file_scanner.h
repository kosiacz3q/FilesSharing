#pragma once

#include <boost/filesystem.hpp>
#include <fstream>
#include <core/string_view.hpp>
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

class FileScanner {
public:
    using TimeStampType = time_t;
    static_assert(std::is_same<TimeStampType, decltype(time(0))>(), "");
    static_assert(std::is_same<TimeStampType, decltype(fs::last_write_time({}))>(), "");

    FileScanner(core::string_view path);
    static std::vector<char> getFileAsBytes(const std::string& path);

    auto getPath() const { return mPath; }
    const auto& getFileInfo() const { return mFiles; }


private:
    core::string_view mPath;
    std::vector<std::pair<std::string, TimeStampType>> mFiles;
};