//
// Created by yanick on 2020/4/16.
//


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#ifndef _WIN32

#include <netinet/in.h>
#include <arpa/inet.h>

# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif

#include <sys/socket.h>

#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include "map.h"

static const char MESSAGE[] = "Welcome...\n";

static const int PORT = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
                        struct sockaddr *, int socklen, void *);

static void conn_writecb(struct bufferevent *, void *);

static void conn_eventcb(struct bufferevent *, short, void *);

static void conn_readcb(struct bufferevent *, void *);

static void signal_cb(evutil_socket_t, short, void *);

static void broadcast(char *,void *);

typedef struct user_data {
    int32_t ip;
} *ud;


int main(int argc, char **argv) {

    // set out buffer clear
    setbuf(stdout, NULL);

    struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;

    struct sockaddr_in sin = {0};
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    //初始化 event base
    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);


    // 设置监听回调
    listener = evconnlistener_new_bind(base, listener_cb, (void *) base,
                                       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                       (struct sockaddr *) &sin,
                                       sizeof(sin));

    if (!listener) {
        fprintf(stderr, "Could not create a listener!\n");
        return 1;
    }

    printf("server ready.. listening at: %d \n", PORT);

    //设置中断信息处理
    signal_event = evsignal_new(base, SIGINT, signal_cb, (void *) base);

    if (!signal_event || event_add(signal_event, NULL) < 0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("done. server will stop\n");
    return 0;

}

static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
            struct sockaddr *sa, int socklen, void *user_data) {
    struct event_base *base = user_data;
    struct bufferevent *bev;

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }

    struct in_addr addr = (((struct sockaddr_in *) sa))->sin_addr;

    fprintf(stdout, "get connection from %s\n", inet_ntoa(addr));

    ud ud = malloc(sizeof(struct user_data));

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, ud);
    bufferevent_enable(bev, EV_WRITE | EV_READ);
//    bufferevent_disable(bev, EV_READ);

    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));

    Client client = (struct client *) malloc(sizeof(struct client));
    client->bev = bev;
    client->ip = addr.s_addr;
    add(client);

    ud->ip = addr.s_addr;
}

static void
conn_readcb(struct bufferevent *bev, void *user_data) {
    uint8_t data[8192];
    size_t n;

    for (;;) {
        n = bufferevent_read(bev, data, sizeof(data));
//        fprintf(stdout, "message: %s", &data);
        bufferevent_write(bev, &data, n);
        if (n <= 0) {
            break;
        }
    }

}


static void
conn_writecb(struct bufferevent *bev, void *user_data) {
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
//        printf("flushed answer\n");
        ///bufferevent_free(bev);
    }
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data) {
    if (events & BEV_EVENT_EOF) {
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Got an error on the connection: %s\n",
               strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    ud ud = user_data;
    bufferevent_free(bev);
    Client c = rem(ud->ip);
    free(ud);
    free(c);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data) {
    struct event_base *base = user_data;
    struct timeval delay = {2, 0};

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}


static void broadcast(char *msg,void *user_data){
    for (int i = 0; i< MAX_CLIENTS; ++i){

    }
}