#include "client_api.h"

#include "common/file_scanner.h"

SendFileToServer::SendFileToServer(uint32_t id, const std::string& path, const std::string& root,
                                   time_t timestamp)
        : Api(type, 0, id)
        , mPath(path)
        , mRoot(root)
        , mTimestamp(timestamp) {
    auto fullName = FileScanner::joinPaths(root, path);
    auto file = FileScanner::getFileAsBytes(fullName);
    setPayload(join_vectors({::to_bytes(mPath, mTimestamp), file}));
}

SendFileToServer::SendFileToServer(const std::vector<char>& bytes)
        : Api(bytes) {
    assert(getType() == type);

    setPayload({bytes.begin() + Api::PayloadInBytesOffset, bytes.end()});
}

void SendFileToServer::setPayload(std::vector<char> payload) {
    Api::setPayload(std::move(payload));
    const auto end = getPayload().end();
    auto it = from_bytes(getPayload().begin(), end, mPath);
    it = from_bytes(it, end, mTimestamp);
    mFileBegin = it;
    assert(getPayload().back() == '\0');
    mFileEnd = getPayload().end();
    assert(mFileBegin <= mFileEnd); // File can be empty, thus begin == end.
}

std::vector<char> SendFileToServer::getFile() const {
    return {mFileBegin, mFileEnd};
}