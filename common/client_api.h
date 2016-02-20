#pragma once

#include <tuple>
#include "common/api.h"

class GetTime : public Api {
public:
    static constexpr char type = 32;

    GetTime(uint32_t id) : Api(type, 0, id) {}
    GetTime(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "GetTime"; }
};

class GetFileList : public Api {
public:
    static constexpr char type = 34;

    GetFileList(uint32_t id) : Api(type, 0, id) {}
    GetFileList(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "GetFileList"; }
};

class GetFileByPath : public Api {
public:
    static constexpr char type = 36;

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

class SendFileToServer : public Api {
public:
    static constexpr char type = 38;

    SendFileToServer(uint32_t id, core::string_view path, const std::vector<char>& bytes)
            : Api(type, 0, id) {
                setPayload(join_vectors({::to_bytes(path), {'\0'}, bytes}));
    }

    SendFileToServer(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    void setPayload(std::vector<char> payload) override {
        Api::setPayload(payload);
    }

    void getFile(std::vector<char>::const_iterator& fileBegin, std::vector<char>::const_iterator& fileEnd) {

        for (fileBegin = getPayload().begin(); fileBegin != getPayload().end(); ++fileBegin) {
            if (*fileBegin == '\0'){
                ++fileBegin;
                return;
            }
        }

        fileEnd = getPayload().end();

        assert(fileEnd != fileBegin);
    }

    std::string getPath() {

        auto pathBegin = getPayload().begin();
        auto pathEnd = pathBegin;

        for (; pathEnd != getPayload().end(); ++pathEnd) {
            if (*pathEnd == '\0'){
                --pathEnd;
                break;
            }
        }

        assert(fileEnd != fileBegin);

        return std::string(pathBegin, pathEnd);
    }

    core::string_view getName() const override { return "SendFileToServer"; }
};

using ClientApiList = std::tuple<GetTime, GetFileList, GetFileByPath>;

