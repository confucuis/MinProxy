#include <signal.h>

#include "proxy.h"

int main(int argc, const char **argv)
{
    if (argc != 5)
    {
        printf("Usage: %s <server_ip> <server_port> <target_ip> <target_port>\n",
               argv[0]);
        exit(1);
    }
    // 设置信号处理程序
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // 实例化代理
    proxy.server_ip = argv[1];
    proxy.target_ip = argv[3];
    proxy.server_port = atoi(argv[2]);
    proxy.target_port = atoi(argv[4]);

    proxy_init(&proxy);
    proxy_run(&proxy);

    return 0;
}