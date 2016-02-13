#pragma once

#include "common/api.h"

class GetTime : public Api {
    static constexpr char type = 32;
public:
    GetTime(uint32_t id) : Api(type, 0, id) {}
    GetTime(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "GetTime"; }
};