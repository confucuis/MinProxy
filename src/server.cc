#include <stdio.h>

#include "server.h"

TCPServer::TCPServer(int port)
{
    this->port = port;
}

TCPServer::~TCPServer(void)
{

}

void TCPServer::init_tcpserver(void)
{
    printf("%d\n", this->port);
}
