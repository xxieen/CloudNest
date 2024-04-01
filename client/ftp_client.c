#include "head.h"
#include "clientTransferFile.h"
int split(char *buf, char **command);
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

  while (1)
  {
    char buf[100] = {0};
    printf("please input your command: ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) != NULL)
    {
      // 移除换行符
      buf[strcspn(buf, "\n")] = '\0';

      // 处理命令
      char *command[10];
      split(buf, command);
      // 连接服务器，发送命令
      send(sockFd, command, sizeof(command), 0);
    }
    else
    {
      // 处理读取错误
      printf("Error reading input\n");
      continue;
    }
  }

  close(sockFd);
  return 0;
}