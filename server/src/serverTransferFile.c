// 小火车协议发送文件
#include "serverTransferFile.h"
// 确保每次接收确保收到len个字节 -> MSG_WAITALL
int recvn(int sockFd, void *pstart, int len)
{
  int total = 0;
  int ret;
  // 强行转为char* 使得指针偏移一次便宜一个字节
  char *p = (char *)pstart;
  while (total < len)
  {
    // 收到len为止
    ret = recv(sockFd, p + total, len - total, 0);
    total += ret;
  }
  return 0;
}

// 发送文件
int serverSendFile(int netFd, char *fileName)
{
  int fileFd = open(fileName, O_RDWR);
  ERROR_CHECK(fileFd, -1, "open");
  train_t train;                   // 定义小火车
  train.length = sizeof(fileName); // 文件名大小
  strcpy(train.buf, fileName);     // 文件名
  int ret = send(netFd, &train, sizeof(train.length) + train.length, 0);
  ERROR_CHECK(ret, -1, "send");
  struct stat statbuf; // 获取文件大小
  ret = fstat(fileFd, &statbuf);
  ERROR_CHECK(ret, -1, "fstat");
  int fileSize = statbuf.st_size;
  memset(&train, 0, sizeof(train)); // 清空火车
  train.length = sizeof(int);       // fileSize
  memcpy(train.buf, &fileSize, sizeof(int));
  ret = send(netFd, &train, sizeof(train.length) + train.length, 0); // 发送文件大小
  ERROR_CHECK(ret, -1, "send");
  char *p = (char *)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd, 0);
  ret = send(netFd, p, fileSize, 0);
  ERROR_CHECK(ret, -1, "send");
  // 结束
  close(fileFd);
  close(netFd);
  munmap(p, fileSize);
  return 0;
}

// 接收文件
int serverRecvFile(int sockFd)
{
  // 接收文件名
  char fileName[1000] = {0};
  int dataLength = 0;
  recvn(sockFd, &dataLength, sizeof(int));
  recvn(sockFd, fileName, dataLength);
  // 打开fileName准备写入数据
  int fileFd = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0666);
  ERROR_CHECK(fileFd, -1, "open");

  // 接收文件大小
  int fileSize = 0;
  recvn(sockFd, &dataLength, sizeof(int)); // 收火车头
  recvn(sockFd, &fileSize, dataLength);    // 收文件大小

  // 给file分配大小
  ftruncate(fileFd, fileSize); // 先将文件改为指定大小，才能mmap
  // mmap给file映射内存空间
  char *p = (char *)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd, 0);
  ERROR_CHECK(p, MAP_FAILED, "mmap");
  // 接收文件内容
  recvn(sockFd, p, fileSize);
  // 结束
  munmap(p, fileSize);
  close(fileFd);
  close(sockFd);
  return 0;
}