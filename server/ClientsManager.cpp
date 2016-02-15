#include "ClientsManager.h"

#include <algorithm>

#include <common/utils.h>


auto comparision = [](const ClientSocketManager& x,const ClientSocketManager& y){ return x.getClientId() < y.getClientId(); };

ClientsManager::ClientsManager()
    :clients(std::set<ClientSocketManager, bool (*)(const ClientSocketManager&, const ClientSocketManager&)>(comparision)){

}

void ClientsManager::AddClient(const int socketFd) {

    mutex_guard  _(exlusiveClientsListAccess);
}

void ClientsManager::removeClient(const int clientId) {
    mutex_guard  _(exlusiveClientsListAccess);

    auto f = std::lower_bound(clients.begin(), clients.end(), clientId, [](const auto& item, const int r){return item.getClientId() > r;});

    if (f == clients.end()){
        perror("Cannot remove non existing client");
        return;
    }

    clients.erase(f);
}

void ClientsManager::disconectAll() {
    mutex_guard  _(exlusiveClientsListAccess);

    for(auto clientManager : clients){
        clientManager.stop();
    }
}
