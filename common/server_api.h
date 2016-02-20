#pragma once

#include "../common/api.h"
#include <time.h>

class ServerTime : public Api {
public:
    static constexpr char type = 33;

    ServerTime(uint32_t id) : Api(type, 0, id), mTimestamp(time(0)) {}
    ServerTime(const std::vector<char>& bytes);

    core::string_view getName() const override { return "ServerTime"; }
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

using ServerApiList = std::tuple<ServerTime, ServerFileList, FileFromServer>;
