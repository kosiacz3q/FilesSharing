#include "communication_manager.h"

#include <iostream>

#include "client_api.h"

void CommunicationManager::send(const Api& message) {
    mSocketManager.push(message.to_bytes());
    std::cerr << "Message:\t{";
    for (auto x : message.to_bytes()) std::cerr << +x << ", ";
    std::cerr << "}\n";
}

std::unique_ptr<Api> CommunicationManager::receive(uint32_t id) {

    refreshMessages();

    for (auto it = mIncoming.begin(); it != mIncoming.end(); ++it) {
        if ((*it)->getID() == id) {
            auto ret = std::move(*it);
            mIncoming.erase(it);
            return ret;
        }
    }
    return nullptr;
}

std::unique_ptr<Api> CommunicationManager::receiveNext(){

    refreshMessages();

    if (mIncoming.empty())
        return nullptr;

    auto ret = std::move(*mIncoming.begin());
    mIncoming.erase(mIncoming.begin());
    return ret;
}

void CommunicationManager::refreshMessages(){
    while (!mSocketManager.isIncomingBufferEmpty()) {
        auto message = mSocketManager.pop();
        Api api(message);
        const char apiType = api.getType();
        bool added = false;
        using Apis = static_for_each<ClientApiList, type_wrapper>;
        for_each_tuple(Apis{}, [&] (auto x) {
            using T = typename decltype(x)::type;
            if (T::type == apiType) {
                assert(!added && "One message cannot have multiple types");
                mIncoming.emplace_back(std::make_unique<T>(message));
                added = true;
            }
        });
    }
}

int CommunicationManager::getId() const{
    return mSocketManager.getClientId();
}