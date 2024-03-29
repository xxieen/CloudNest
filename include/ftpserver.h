#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
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
