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

   DEFAULT_MOVE_COPY(Api);

    virtual std::string to_string() const;
    virtual std::vector<char> to_bytes() const;

    virtual void dump() const;
    virtual void dumpPayload() const;
    virtual core::string_view getName() const { return "Api"; }

    auto getType() const { return mType; }
    void setStatus(const uint8_t newStatus){
        mStatus = newStatus;
    }

    auto getStatus() const { return mStatus; }
    auto getID() const { return mID; }
    const auto& getPayload() const { return mPayload; }

protected:
    virtual void setPayload(std::vector<char> payload);

private:
    uint8_t mType = 0;
    uint8_t mStatus = 0;
    uint32_t mID = 0;
    std::vector<char> mPayload;

public:
    static constexpr size_t PayloadInBytesOffset = sizeof(uint8_t) + sizeof(uint8_t)
                                                   + sizeof(uint32_t) + sizeof(uint32_t);
};

