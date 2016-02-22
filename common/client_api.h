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
        mPath = std::string(payload.data());
    }

    core::string_view getPath() const { return  mPath; }
    core::string_view getName() const override { return "GetFileByPath"; }

protected:

    void setPayload(std::vector<char> payload) override {
        Api::setPayload(payload);
        mPath = std::string(payload.data());
    }

private:
    std::string mPath;
};

class SendFileToServer : public Api {
public:
    static constexpr char type = 38;

    SendFileToServer(uint32_t id, const std::string& path, const std::string& root,
                     time_t timestamp);
    SendFileToServer(const std::vector<char>& bytes);

    std::vector<char> getFile() const;
    const auto& getPath() const { return mPath; }
    auto getTimestamp() const { return mTimestamp; }

    core::string_view getName() const override { return "SendFileToServer"; }

protected:

    void setPayload(std::vector<char> payload) override;

private:
    std::string mPath;
    std::string mRoot;
    time_t mTimestamp;
    std::vector<char>::const_iterator mFileBegin;
    std::vector<char>::const_iterator mFileEnd;
};

class GetDeletedList : public Api {
public:
    static constexpr char type = 40;

    GetDeletedList(uint32_t id) : Api(type, 0, id) {}
    GetDeletedList(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "GetDeletedList"; }
};

class MarkAsDeleted : public Api {
public:
    static constexpr char type = 42;

    MarkAsDeleted(uint32_t id, const std::string& path) : Api(type, 0, id), mPath(path) {
        setPayload(::to_bytes(path));
    }
    MarkAsDeleted(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
        from_bytes(getPayload().begin(), getPayload().end(), mPath);
    }

    const auto& getPath() const { return mPath; }
    core::string_view getName() const override { return "GetDeletedList"; }

private:
    std::string mPath;
};

using ClientApiList = std::tuple<GetTime, GetFileList, GetFileByPath, GetDeletedList,
                                 MarkAsDeleted>;
