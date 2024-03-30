#include "./include/head.h"
typedef struct tran_s
{
  int length;     // 火车头
  char buf[1000]; // 有效载荷
} train_t;
int recvn(int sockFd, void *pstart, int len);
int serverSendFile(int netFd, char *fileName);
int serverRecvFile(int sockFd);