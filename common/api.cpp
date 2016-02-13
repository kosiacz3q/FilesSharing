#include "api.h"

#include <sstream>
#include <iostream>

Api::Api(const std::vector<char>& bytes) {
    auto it = from_bytes(bytes.begin(), bytes.end(), mType, mStatus, mID);
    mPayload = std::vector<char>(it, bytes.end());
}

std::string Api::to_string() const {
    std::ostringstream oss;
    oss << "Api:\t" <<  getName() << " type:\t" << mType << " status:\t" << mStatus;
    oss << " it:\t" << mID << " size:\t" << mPayload.size();
    return oss.str();
}

std::vector<char> Api::to_bytes() const {
    auto temp = ::to_bytes(mType, mStatus, mID);
    return join_vectors({temp, mPayload});
}

void Api::dump() const {
    std::cerr << to_string() << "\n";
}