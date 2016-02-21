#include "client_logic.h"

#include <iostream>

#include "common/file_scanner.h"

ClientLogic::ClientLogic(CommunicationManager& cm, const std::string& rootFolder)
        : mCM(cm)
        , mRoot(rootFolder) {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\n";
    std::cout << "Starting client. Root folder: " << mRoot << "\n";

}