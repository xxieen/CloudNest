#include "head.h"
#include "clientTransferFile.h"
// ./client 10.0.8.3 1234
int main(int argc, char *argv[])
{
  ARGS_CHECK(argc, 3);
  int sockFd = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  int ret = connect(sockFd, (struct sockaddr *)&addr, sizeof(addr));
  ERROR_CHECK(ret, -1, "connect");
  clientDownloadFile(sockFd);
  close(sockFd);
  return 0;
}