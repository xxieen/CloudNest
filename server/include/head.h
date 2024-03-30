#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
using namespace std;

#define ARGS_CHECK(argc, num)           \
  {                                     \
    if (argc != num)                    \
    {                                   \
      fprintf(stderr, "args error!\n"); \
      return -1;                        \
    }                                   \
  }

#define ERROR_CHECK(ret, num, msg) \
  {                                \
    if (ret == num)                \
    {                              \
      perror(msg);                 \
      return -1;                   \
    }                              \
  }

#define THREAD_ERROR_CHECK(ret, msg)                  \
  {                                                   \
    if (ret != 0)                                     \
    {                                                 \
      fprintf(stderr, "%s:%s\n", msg, strerror(ret)); \
    }                                                 \
  }
