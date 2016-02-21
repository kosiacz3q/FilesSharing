#include "client_api.h"

#include <algorithm>

SendFileToServer::SendFileToServer(uint32_t id, const std::string& path, const std::string& root,
                                   time_t timestamp, const std::vector<char>& file)
        : Api(type, 0, id)
        , mPath(path)
        , mRoot(root)
        , mTimestamp(timestamp) {
    setPayload(join_vectors({::to_bytes(mPath, mTimestamp), file}));
}

void SendFileToServer::setPayload(std::vector<char> payload) {
    Api::setPayload(std::move(payload));
    //from_bytes(getPayload().begin(), getPayload().end(), )
}