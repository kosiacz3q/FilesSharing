#pragma once

#include "../common/api.h"
#include <ctime>

class ServerTime : public Api {
public:
    static constexpr char type = 33;

    ServerTime(uint32_t id);
    ServerTime(const std::vector<char>& bytes);

    core::string_view getName() const override { return "ServerTime"; }

    auto getTimestamp() const { return mTimestamp; }
private:
    time_t mTimestamp;
};

class ServerFileList : public Api {
public:
    static constexpr char type = 35;

    ServerFileList(uint32_t id) : Api(type, 0, id) {}
    ServerFileList(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "ServerFileList"; }
};

class FileFromServer : public Api {
public:
    static constexpr char type = 37;

    FileFromServer(uint32_t id) : Api(type, 0, id) {}
    FileFromServer(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "RequestedFile"; }
};

class FileFromClient : public Api {
public:
    static constexpr char type = 39;

    FileFromClient(uint32_t id) : Api(type, 0, id) {}
    FileFromClient(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "ClientFile"; }
};

using ServerApiList = std::tuple<ServerTime, ServerFileList, FileFromServer>;
