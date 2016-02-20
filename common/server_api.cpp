#include "../common/server_api.h"

ServerTime::ServerTime(const std::vector<char>& bytes)
        : Api(bytes) {
    assert(getType() == type);
    assert(getPayload().size() > 0 && "No timestamp data");
    (void) from_bytes(getPayload().begin(), getPayload().end(), mTimestamp);
}