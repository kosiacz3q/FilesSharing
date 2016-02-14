#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <core/string_view.hpp>
#include "common/utils.h"

class Api {
public:
    virtual ~Api() = default;

    Api(uint8_t type, uint8_t status, uint32_t id)
            : mType(type), mStatus(status), mID(id) {}

    Api(uint8_t type, uint8_t status, uint32_t id, const std::vector<char>& payload)
            : mType(type), mStatus(status), mID(id), mPayload(payload) {}

    Api(const std::vector<char>& bytes);

    Api(const Api&) = default;
    Api& operator=(const Api&) = default;
    Api(Api&&) = default;
    Api& operator=(Api&&) = default;

    virtual std::string to_string() const;
    virtual std::vector<char> to_bytes() const;
    virtual void setPayload(std::vector<char> payload);
    virtual void dump() const;
    virtual core::string_view getName() const { return "Api"; }

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

