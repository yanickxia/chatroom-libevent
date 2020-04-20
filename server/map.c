//
// Created by yanick on 2020/4/20.
//

#include <stdint.h>
#include <stdio.h>
#include "map.h"

static void *clients[MAX_CLIENTS];
static size_t count = 0;

int add(Client client) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (NULL == clients[i]) {
            clients[i] = client;
            count++;
            return 0;
        }
    }

    return -1;
}

Client rem(int32_t ip) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        Client client = ((Client) clients[i]);
        if (client->ip == ip) {
            clients[i] = NULL;
            count--;
            return client;
        }
    }
    return NULL;
}

Client client_list() {
    struct client *cs[count];
    size_t index = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != NULL) {
            cs[index] = (struct client *) clients[i];
            index++;
        }
    }

    return cs[0];
}