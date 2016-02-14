#include "communication_manager.h"

void CommunicationManager::send(const Api& message) {
    mSocketManager.push(message.to_bytes());
}

std::unique_ptr<Api> CommunicationManager::receive(uint32_t id) {
    while (!mSocketManager.isIncomingBufferEmpty()) {
        auto message = mSocketManager.pop();
        mIncoming.emplace_back(std::make_unique<Api>(message)); // TODO: finish dynamic type matching
    }

    for (auto it = mIncoming.begin(); it != mIncoming.end(); ++it) {
        if ((*it)->getID() == id) {
            auto ret = std::make_unique<Api>(std::move(**it));
            mIncoming.erase(it);
            return ret;
        }
    }
    return nullptr;
}