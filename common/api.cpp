#include "api.h"

#include <sstream>
#include <iostream>

Api::Api(const std::vector<char>& bytes) {
    uint32_t payloadSize;
    auto it = from_bytes(bytes.begin(), bytes.end(), mType, mStatus, mID, payloadSize);
    mPayload = std::vector<char>(it, bytes.end());
}

std::string Api::to_string() const {
    std::ostringstream oss;
    oss << "Api:\t" <<  getName() << " type:\t" << +mType << " status:\t" << +mStatus;
    oss << " id:\t" << mID << " size:\t" << mPayload.size();
    return oss.str();
}

std::vector<char> Api::to_bytes() const {
    uint32_t size(mPayload.size());
    auto temp = ::to_bytes(mType, mStatus, mID, size);
    return join_vectors({temp, mPayload});
}

void Api::dump() const {
    std::cerr << to_string() << "\n";
}

void Api::setPayload(std::vector<char> payload) {
    mPayload = std::move(payload);
}
