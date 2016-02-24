//
// Created by lucas on 2/20/16.
//

#ifndef SERVER_SENDFILE_H
#define SERVER_SENDFILE_H


#include "common/client_api.h"
#include "IAction.h"

class SendFile : public IAction{

public:

    int getType() const {
        return GetFileByPath::type;
    }

    void handle(CommunicationManagerPtr, std::unique_ptr<Api> msg);
};


#endif //SERVER_SENDFILE_H
