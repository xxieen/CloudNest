#include "../include/head.h"

// 每一个任务
typedef struct task_s
{
    int netFd;
    struct task_s *pNext;
} task_t;
/* task queue 任务队列，这个结构体里面都是进程池里面的进程要拿的资源，
而且是共享的，所以把互斥锁和信号量放在这里比较合适 */
typedef struct taskQueue_s
{
    task_t *pFront;
    task_t *pRear;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} taskQueue_t;

// 线程池
typedef struct threadPool_s
{
    pthread_t *tid;
    int threadNum;
    taskQueue_t taskQueue;
} threadPool_t;

// 主线程需要的函数
int pThreadPoolInit(threadPool_t *pthreadPool, int workerNum);
int makeWorkers(threadPool_t *threadPoo, int workerNum);
void *thread_function(void *arg);

// 队列相关函数
int taskEnqueue(taskQueue_t *pTaskQueue, int netFd);
int taskDequeue(taskQueue_t *pTaskQueue);

// tcp相关函数
int tcpInit(int *psockFd, char *ip, char *port);

// epoll相关函数
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);