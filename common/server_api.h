#ifndef SERVER_SERVER_API_H
#define SERVER_SERVER_API_H

class SendTime : public Api {
public:
    static constexpr char type = 33;

    SendTime(uint32_t id) : Api(type, 0, id) {}
    SendTime(const std::vector<char>& bytes) : Api(bytes) {
        assert(getType() == type);
    }

    core::string_view getName() const override { return "ServerTime"; }
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

#endif //SERVER_SERVER_API_H_H
