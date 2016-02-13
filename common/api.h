#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "common/utils.h"

class Api {
public:
    virtual ~Api() = default;

    Api(uint8_t type, uint8_t status, uint32_t id)
            : mType(type), mStatus(status), mID(id) {}

    Api(uint8_t type, uint8_t status, uint32_t id, const std::vector<char>& payload)
            : mType(type), mStatus(status), mID(id), mPayload(payload) {}

    Api(const std::vector<char>& bytes);

    virtual std::string to_string() const;
    virtual std::vector<char> to_bytes() const;
    virtual void dump() const;

    auto getType() const { return mType; }
    auto getStatus() const { return mStatus; }
    auto getID() const { return mID; }
    const auto& getPayload() const { return mPayload; }

private:
    uint8_t mType = 0;
    uint8_t mStatus = 0;
    uint32_t mID = 0;
    std::vector<char> mPayload;
};