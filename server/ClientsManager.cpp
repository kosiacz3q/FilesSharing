#include "ClientsManager.h"

auto comparision = [](const ClientHandlerPtr& x,const ClientHandlerPtr& y){ return x->getId() < y->getId(); };

ClientsManager::ClientsManager()
    :clients(std::set<ClientHandlerPtr, bool (*)(const ClientHandlerPtr&, const ClientHandlerPtr&)>(comparision)){

}

void ClientsManager::AddClient(ClientHandlerPtr newClient) {

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
