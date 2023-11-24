#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

void error_handling(char *message);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <port> <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");

    int target_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (target_sock == -1)
        error_handling("socket() error");

    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(argv[2]);
    target_addr.sin_port = htons(atoi(argv[3]));

    if (connect(target_sock, (struct sockaddr *)&target_addr, sizeof(target_addr)) == -1)
        error_handling("connect() error");

    char buffer[BUFFER_SIZE];
    int recv_len;

    while ((recv_len = read(clnt_sock, buffer, sizeof(buffer))) != 0)
    {
        if (recv_len == -1)
            error_handling("read() error");

        write(target_sock, buffer, recv_len);

        int send_len = read(target_sock, buffer, sizeof(buffer));
        if (send_len == -1)
            error_handling("read() error");

        write(clnt_sock, buffer, send_len);
    }

    close(clnt_sock);
    close(serv_sock);
    close(target_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
