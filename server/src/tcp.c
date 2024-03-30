#include "../include/head.h"
#include "threadPool.h"

// 主线程初始化tcp连接
int tcpInit(int *psockFd, char *ip, char *port)
{
    // socket
    *psockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*psockFd, -1, "socket");
    // bind(首先要把ip地址和端口号存进一个结构体里面去)
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htoi(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(*psockFd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    // listen
    listen(*psockFd, 10);
}