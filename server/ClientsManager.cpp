#include "ClientsManager.h"

#include <algorithm>

#include <common/utils.h>
#include <common/communication_manager.h>

auto comparision = [](const CommunicationManagerPtr& x,const CommunicationManagerPtr& y){ return x->getId() < y->getId(); };

ClientsManager::ClientsManager()
    :clients(std::set<CommunicationManagerPtr, bool (*)(const CommunicationManagerPtr&, const CommunicationManagerPtr&)>(comparision)){

}

void ClientsManager::AddClient(CommunicationManagerPtr newClient) {

    mutex_guard  _(exlusiveClientsListAccess);

    clients.insert(newClient);
}

void ClientsManager::removeClient(const int clientId) {
    mutex_guard  _(exlusiveClientsListAccess);

    auto f = std::lower_bound(clients.begin(), clients.end(), clientId, [](const auto& item, const int r){return item->getId() < r;});

    if (f == clients.end()){
        perror("Cannot remove non existing client");
        return;
    }

    clients.erase(f);
}

void ClientsManager::disconectAll() {
    mutex_guard  _(exlusiveClientsListAccess);

    clients.clear();
}
