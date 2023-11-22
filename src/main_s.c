// server
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET_ERR -1
#define BIND_ERR -2
#define LISTEN_ERR -3
#define ACCEPT_ERR -4
#define RECV_ERR -5

#define SERVER_PORT 8000
#define SERVER_HOST "127.0.0.1"
#define BUFFER 128

int main(void)
{
    // 创建socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("创建socket失败\n");
        exit(SOCKET_ERR);
    }

    // 设置socket信息
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(SERVER_HOST);
    socket_address.sin_port = htons(SERVER_PORT);
    // 绑定socket到地址
    if (bind(sock_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        perror("绑定socket失败\n");
        close(sock_fd);
        exit(BIND_ERR);
    }

    // 监听
    if (listen(sock_fd, 10) < 0)
    {
        perror("监听socket失败\n");
        close(sock_fd);
        exit(LISTEN_ERR);
    }

    printf("服务器启动: %s:%d\n", SERVER_HOST, SERVER_PORT);
    // 等待连接
    for (;;)
    {
        int conn_fd = accept(sock_fd, NULL, NULL);
        if (conn_fd < 0)
        {
            perror("等待客户端连接错误\n");
            close(sock_fd);
            exit(ACCEPT_ERR);
        }

        printf("客户端连接成功......\n");
        char buffer[BUFFER];
        size_t read_bytes;
        for (read_bytes = recv(conn_fd, buffer, sizeof(buffer), 0); read_bytes > 0;)
        {
            printf("接收的数据: %.*s\n", (int)read_bytes, buffer);
            // 回显数据
            send(conn_fd, buffer, read_bytes, 0);
            break;
        }
        if (read_bytes < 0)
        {
            perror("接收客户端数据出错\n");
            close(conn_fd);
            exit(RECV_ERR);
        }
        close(conn_fd);
    }
    // 关闭套接字
    close(sock_fd);

    return 0;
}