// 服务器接口

#ifndef _SERVER_H_
#define _SERVER_H_

class TCPServer
{
private:
    int port;
    const char *host = "0.0.0.0";

public:
    TCPServer(int port);
    ~TCPServer(void);
    void init_tcpserver(void);
};

#endif // _SERVER_H_