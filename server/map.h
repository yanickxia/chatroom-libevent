//
// Created by yanick on 2020/4/20.
//

#ifndef CHATROOM_MAP_H
#define CHATROOM_MAP_H

#define MAX_CLIENTS 1000


typedef struct client {
    int32_t ip;
    struct bufferevent* bev;
} *Client;


int add(Client);

Client rem(int32_t);

#endif //CHATROOM_MAP_H
