// client
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET_ERR -1
#define CONN_ERR -2
#define SEND_ERR -3
#define RECV_ERR -4

#define SERVER_PORT 8000
#define SERVER_HOST "127.0.0.1"
#define BUFFER 128

int main(void)
{
    // 创建套接字
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("创建套接字失败");
        exit(SOCKET_ERR);
    }

    // 设置套接字
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(SERVER_HOST);
    socket_address.sin_port = htons(SERVER_PORT);

    // 连接套接字
    int c_socket = connect(sock_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if (c_socket < 0)
    {
        perror("连接套接字报错\n");
        close(sock_fd);
        exit(CONN_ERR);
    }

    // 发送消息
    char message[] = "zhangsan, xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx[test-server-client]xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    if (send(sock_fd, message, strlen(message), 0) < 0)
    {
        perror("发送消息失败\n");
        close(c_socket);
        exit(SEND_ERR);
    }

    // 接收消息
    char buffer[BUFFER];
    size_t read_bytes = recv(sock_fd, buffer, sizeof(buffer), 0);
    if (read_bytes < 0)
    {
        perror("接收消息失败\n");
        close(c_socket);
        exit(RECV_ERR);
    }

    printf("接收的消息: %.*s\n", (int)read_bytes, buffer);

    // 关闭套接字
    close(sock_fd);

    return 0;
}