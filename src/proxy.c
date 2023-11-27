#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

typedef struct
{
    int server_socket;
    int client_socket;
    int target_socket;
} Proxy;

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void proxy_init(Proxy *proxy, char *server_ip, int server_port,
                char *target_ip, int target_port)
{
    struct sockaddr_in serv_addr, target_addr;

    proxy->server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (proxy->server_socket == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(server_port);

    if (bind(proxy->server_socket, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(proxy->server_socket, 5) == -1)
        error_handling("listen() error");

    proxy->client_socket = accept(proxy->server_socket, NULL, NULL);
    if (proxy->client_socket == -1)
        error_handling("accept() error");

    proxy->target_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (proxy->target_socket == -1)
        error_handling("socket() error");

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);
    target_addr.sin_port = htons(target_port);

    if (connect(proxy->target_socket, (struct sockaddr *)&target_addr,
                sizeof(target_addr)) == -1)
        error_handling("connect() error");
}

void proxy_run(Proxy *proxy)
{
    char buffer[BUFFER_SIZE];
    int recv_len;

    while ((recv_len =
                read(proxy->client_socket, buffer, sizeof(buffer))) != 0)
    {
        if (recv_len == -1)
            error_handling("read() error");

        write(proxy->target_socket, buffer, recv_len);

        int send_len = read(proxy->target_socket, buffer, sizeof(buffer));
        if (send_len == -1)
            error_handling("read() error");

        write(proxy->client_socket, buffer, send_len);
    }
}

void proxy_cleanup(Proxy *proxy)
{
    close(proxy->client_socket);
    close(proxy->server_socket);
    close(proxy->target_socket);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <server_port> <target_ip> <target_port>\n",
               argv[0]);
        exit(1);
    }

    Proxy proxy;
    proxy_init(&proxy, "127.0.0.1", atoi(argv[1]), argv[2], atoi(argv[3]));

    proxy_run(&proxy);

    proxy_cleanup(&proxy);

    return 0;
}
