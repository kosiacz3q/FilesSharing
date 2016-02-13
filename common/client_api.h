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

class GetFileList : public Api {
    static constexpr char type = 34;
public:
    GetFileList(uint32_t id) : Api(type, 0, id) {}
    GetFileList(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "GetFileList"; }
};

class GetFileByPath : public Api {
    static constexpr char type = 36;
public:
    GetFileByPath(uint32_t id, core::string_view path)
            : Api(type, 0, id), mPath(path) {
        setPayload(::to_bytes(path));
    }
    GetFileByPath(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
        auto& payload = getPayload();
        mPath = std::string((const char *) payload.data());
    }

    void setPayload(std::vector<char> payload) override {
        Api::setPayload(payload);
        mPath = std::string((const char *) payload.data());
    }

    core::string_view getPath() const { return  mPath; }
    core::string_view getName() const override { return "GetFileByPath"; }

private:
    std::string mPath;
};

