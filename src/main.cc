#include <stdio.h>

#include "server.h"

int main(void)
{
    TCPServer tcp_server(8000);
    tcp_server.init_tcpserver();
    return 0;
}