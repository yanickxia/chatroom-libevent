//
// Created by yanick on 2020/4/20.
//

#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

#include <map>
#include <vector>
#include <string>


class Client {
public:
    std::string id;
    void *bev;
};

class Clients {
private:
    std::map<std::string, Client> clients;
public:
    Client& get(const std::string& id);
    void add(const Client& client);
    std::vector<Client> online();
};


#endif //CHATROOM_CLIENT_H
