#include "../common/server_api.h"

#include <iostream>

#include "common/file_scanner.h"

ServerTime::ServerTime(uint32_t id) 
		: Api(type, 0, id)
		, mTimestamp(time(nullptr)) {
    setPayload(::to_bytes(mTimestamp));
    std::cerr << "Sent time:\t" << mTimestamp << "\n";
    assert(::to_bytes(mTimestamp) == getPayload());
}

ServerTime::ServerTime(const std::vector<char>& bytes)
        : Api(bytes) {
    assert(getType() == type);
    assert(getPayload().size() != sizeof(time(nullptr)) && "No timestamp data");
    (void) from_bytes(getPayload().begin(), getPayload().end(), mTimestamp);
    std::cerr << "Deserialized time:\t" << mTimestamp << "\n";
}

ServerFileList::ServerFileList(uint32_t id, const std::string& path)
        : Api(type, 0, id) {
    FileScanner fs(path);
    mFileList = fs.asFileList();
    auto bytes = ::to_bytes(mFileList);
    setPayload(bytes);
    std::cerr << "Serialization file list\n" << mFileList << "\n";
}

ServerFileList::ServerFileList(const std::vector<char>& bytes)
        : Api(bytes)
        , mFileList(getPayload().data(), getPayload().data() + getPayload().size()) {
    assert(getType() == type);
    std::cerr << "Deserialization file list:\n" << mFileList << "\n";
}

FileFromServer::FileFromServer(uint32_t id, const std::string& path, const std::string& root)
        : Api(type, 0, id) {
    auto fullPath = FileScanner::joinPaths(path, root);
    assert(FileScanner::exists(fullPath));
    auto bytes = FileScanner::getFileAsBytes(fullPath);
    setPayload(bytes);
}

ServerDeletedList::ServerDeletedList(uint32_t id, const std::vector<std::string>& paths)
        : Api(type, 0, id) {

    std::vector<char> result = {};

    for (auto s : paths)
        result = join_vectors({result, ::to_bytes(s)});

    setPayload(result);
}

ServerDeletedList::ServerDeletedList(const std::vector<char>& bytes): Api(bytes) {
    assert(getType() == type);

    auto payload = getPayload();

    for (auto actual = payload.begin(), lastBegin = actual; actual != payload.end(); ++actual){
        if (*actual == '\0'){
            std::string path;
            from_bytes(lastBegin, actual, path);
            mDeletedList.push_back(path);
            lastBegin = actual + 1;
        }
    }
}