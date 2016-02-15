#include "SendTimeStamp.h"

#include <chrono>

#include <common/server_api.h>


void SendTimeStamp::handle(CommunicationManagerPtr ptr, std::unique_ptr<Api> msg) {

    SendTime st(msg->getID());

    using namespace std::chrono;

    st.setPayload(to_bytes(duration_cast< milliseconds >(
            system_clock::now().time_since_epoch())));

    ptr->send(st);
}

int SendTimeStamp::getType() const {
    return GetTime::type;
}
