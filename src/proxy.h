// proxy

#ifndef _PROXY_H_
#define _PROXY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
    int server_port;
    int target_port;
    const char *server_ip;
    const char *target_ip;

    int server_sock;
    int client_conn;
    int target_sock;
} Proxy;

extern Proxy proxy;

void error_handling(char *msg, int num, ...);
void proxy_init(Proxy *proxy);
void proxy_run(Proxy *proxy);
void sigint_handler(int signo);

#endif // _PROXY_H_