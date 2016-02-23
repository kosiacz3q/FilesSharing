#pragma once

#include <ctime>
#include <common/api.h>
#include <common/utils.h>

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

    ServerFileList(uint32_t id, const std::string& path);
    ServerFileList(const std::vector<char>& bytes);

    core::string_view getName() const override { return "ServerFileList"; }

    const auto& getFileList() const { return mFileList; }

private:
    std::string mFileList;
};

class FileFromServer : public Api {
public:
    static constexpr char type = 37;

    FileFromServer(uint32_t id, const std::string& path, const std::string& root);
    FileFromServer(uint32_t id, uint32_t status);
    FileFromServer(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    const auto& getFile() const { return getPayload(); }

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

class ServerDeletedList : public Api {
public:
    static constexpr char type = 41;

    ServerDeletedList(uint32_t id, const std::vector<std::string>& paths);

    ServerDeletedList(const std::vector<char>& bytes);

    core::string_view getName() const override { return "ServerFileList"; }

    const auto& getDeletedList() const { return mDeletedList; }

private:
    std::vector<std::string> mDeletedList;
};

class ServerDeletedResponse : public Api {
public:
    static constexpr char type = 43;

    ServerDeletedResponse(uint32_t id) : Api(type, 0, id) {}
    ServerDeletedResponse(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "ClientFile"; }
};

using ServerApiList = std::tuple<ServerTime, ServerFileList, FileFromServer, ServerDeletedList,
                                 ServerDeletedResponse, FileFromClient>;
