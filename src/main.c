#include <stdio.h>
#include <stdlib.h>

#include "proxy.h"

int main(int argc, const char *argv)
{
    // if (argc != 5)
    // {
    //     printf("Usage: %s <server_ip> <server_port> <target_ip> <target_port>\n",
    //            argv[0]);
    //     exit(1);
    // }

    Proxy proxy;
    proxy.server_ip = "127.0.0.1";
    proxy.target_ip = "127.0.0.1";
    proxy.server_port = 9999;
    proxy.target_port = 6666;

    proxy_init(&proxy);
    printf("初始化代理成功!\n");
    printf("代理服务启动地址: [%s:%d]\n", proxy.server_ip, proxy.server_port);
    proxy_run(&proxy);
    proxy_clean(&proxy);

    return 0;
}