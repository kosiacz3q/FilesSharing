#pragma once

#include <string>
#include <cstdint>

#include "common/communication_manager.h"

class ClientLogic {
public:
    ClientLogic(CommunicationManager& cm, const std::string& rootFolder);

private:
    enum class Error {
        NoError,
        TimeDiffError,
        Timeout,
    };

    bool checkTimeDiff();

    uint32_t nextID() { return ++mMessageID; }
    uint32_t currentID() { return mMessageID; }

    Error loop();

    CommunicationManager& mCM;
    std::string mRoot;
    uint32_t mMessageID = 0;
    time_t mTimeOffset = 0;
};

