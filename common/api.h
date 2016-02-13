#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "common/utils.h"

class Api {
public:
    virtual ~Api() = default;

    Api(const std::vector<char>& bytes);

    virtual std::string to_string() const;
    virtual std::vector<char> to_bytes() const;
    virtual void dump() const;
private:
    uint8_t mType;
    uint8_t mStatus;
    uint32_t mID;
    std::vector<char> mPayload;
};