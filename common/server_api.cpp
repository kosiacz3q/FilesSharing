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