#pragma once

#include "../common/api.h"
#include <time.h>

class SendTime : public Api {
public:
    static constexpr char type = 33;

    SendTime(uint32_t id) : Api(type, 0, id), mTimestamp(time(0)) {}
    SendTime(const std::vector<char>& bytes);

    core::string_view getName() const override { return "ServerTime"; }
private:
    time_t mTimestamp;
};

class SendFileList : public Api {
public:
    static constexpr char type = 35;

    SendFileList(uint32_t id) : Api(type, 0, id) {}
    SendFileList(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "ServerFileList"; }
};

class SendFile : public Api {
public:
    static constexpr char type = 37;

    SendFile(uint32_t id) : Api(type, 0, id) {}
    SendFile(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "RequestedFile"; }
};

using ServerApiList = std::tuple<SendTime, SendFileList, SendFile>;
