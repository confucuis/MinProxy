#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "proxy.h"

#define BUFFER 1024

void error_handling(char *msg, int num, ...)
{
    perror(msg);
    // fputs(msg, stderr);
    fputc('\n', stderr);
    va_list ap;
    va_start(ap, num);
    for (int i = 0; i < num; ++i)
    {
        close(va_arg(ap, int)); // 从可变参数列表中读取下一个整数
    }
    va_end(ap);

    exit(1);
}

void proxy_init(Proxy *proxy)
{
    struct sockaddr_in server_addr, target_addr;
    // server socket
    if ((proxy->server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("server socket() error", 1, proxy->server_sock);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(proxy->server_port);
    server_addr.sin_addr.s_addr = inet_addr(proxy->server_ip);
    if (bind(proxy->server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        error_handling("server bind() error", 1, proxy->server_sock);
    if (listen(proxy->server_sock, 128) == -1)
        error_handling("server listen() error", 1, proxy->server_sock);

    // target socket
    if ((proxy->target_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("target socket() error", 2, proxy->target_sock, proxy->server_sock);
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(proxy->target_port);
    target_addr.sin_addr.s_addr = inet_addr(proxy->target_ip);
    while (connect(proxy->target_sock, (struct sockaddr *)&target_addr, sizeof(target_addr)) == -1)
    // error_handling("target connect() error", 2, proxy->target_sock, proxy->server_sock);
    {
        printf("等待目标服务启动...\n");
        sleep(2);
    }
    printf("目标服务启动成功, 地址: [%s:%d]\n", proxy->target_ip, proxy->target_port);
}

void proxy_run(Proxy *proxy)
{
    char buffer[BUFFER];
    int recv_len;
    // int send_len;
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);
    printf("代理服务启动地址: [%s:%d]\n", proxy->server_ip, proxy->server_port);
    while (1)
    {
        printf("等待客户端连接...\n");
        // 等待客户端连接
        if ((proxy->client_conn = accept(proxy->server_sock, (struct sockaddr *)&client_addr, &client_size)) < 0)
            error_handling("client accept() error", 2, proxy->client_conn, proxy->server_sock);
        printf("客户端连接成功,开始从客户端读取数据...\n");
        while ((recv_len = read(proxy->client_conn, buffer, sizeof(buffer))) > 0)
        {
            printf("开始往代理服务器转发数据...\n");
            if (write(proxy->target_sock, buffer, recv_len) == -1)
                error_handling("target write() error", 3, proxy->target_sock, proxy->client_conn, proxy->server_sock);
            break;
        }

        // while ((send_len = read(proxy->target_sock, buffer, sizeof(buffer))) > 0)
        // {
        //     if (write(proxy->server_conn, buffer, send_len) == -1)
        //         error_handling("client write() error", 3, proxy->target_sock, proxy->server_conn, proxy->server_sock);
        //     break;
        // }
    }
}

void proxy_clean(Proxy *proxy)
{
    close(proxy->server_sock);
    close(proxy->target_sock);
    close(proxy->client_conn);
}