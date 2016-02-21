#pragma once

#include <string>

#include "common/communication_manager.h"

class ClientLogic {
public:
    ClientLogic(CommunicationManager& cm, const std::string& rootFolder);

private:
    CommunicationManager& mCM;
    std::string mRoot;
};

