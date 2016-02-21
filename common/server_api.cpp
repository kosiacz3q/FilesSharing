#include "../common/server_api.h"

#include <iostream>

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