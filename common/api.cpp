#include "api.h"

#include <sstream>
#include <iostream>

Api::Api(const std::vector<char>& bytes) {
    auto it = bytes.begin();
    from_bytes(mType, it);
    it += sizeof(mType);
    from_bytes(mStatus, it);
    it += sizeof(mStatus);
    from_bytes(mID, it);
    it += sizeof(mID);
    mPayload = std::vector<char>(it, bytes.end());
}

std::string Api::to_string() const {
    std::ostringstream oss;
    oss << "Api. type:\t" << mType << " status:\t" << mStatus << " it:\t" << mID;
    oss << " size:\t" << mPayload.size();
    return oss.str();
}

std::vector<char> Api::to_bytes() const {
    auto type = ::to_bytes(mType);
    auto status = ::to_bytes(mStatus);
    auto id = ::to_bytes(mID);
    return join_vectors({&type, &status, &id, &mPayload});
}

void Api::dump() const {
    std::cerr << to_string() << "\n";
}