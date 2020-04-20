//
// Created by yanick on 2020/4/20.
//

#include "Client.h"

Client &Clients::get(const std::string& id) {
    return this->clients[id];
}

void Clients::add(const Client& client) {
    this->clients[client.id] = client;
}

std::vector<Client> Clients::online() {
    auto cs = std::vector<Client>();
    auto exist_clients = this->clients;
    for (auto &exist_client : exist_clients) {
        cs.push_back(exist_client.second);
    }
    return cs;
}
