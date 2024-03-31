#include "../include/head.h"
#include "threadPool.h"

int main(int argc, char *argv[])
{
    // ./ftp_server 10.0.8.3 1234 4
    ARGS_CHECK(argc, 4);
    int workerNum = atoi(argv[3]);
    // 创建并初始化线程池
    threadPool_t threadPool;
    pThreadPoolInit(&threadPool, workerNum);
    // 在线程池中创建线程
    makeWorkers(&threadPool, workerNum);

    // tcp连接的初始化
    int sockFd;
    tcpInit(&sockFd, argv[1], argv[2]);

    // 创建epoll
    int epfd = epoll_create(1);
    epollAdd(sockFd, epfd);
    struct epoll_event readyArr[2];
    // 不断的等待epoll
    while (1)
    {
        int readyNum = epoll_wait(epfd, readyArr, 2, -1);
        puts("epoll_wait returns");
        for (int i = 0; i < readyNum; ++i)
        {
            if (readyArr[i].data.fd == sockFd)
            {
                int netFd = accept(sockFd, NULL, NULL);
                // 拿到了netFd就准备放进任务队列里面
                pthread_mutex_lock(&threadPool.taskQueue.mutex);
                taskEnqueue(&threadPool.taskQueue, netFd);
                pthread_cond_signal(&threadPool.taskQueue.cond);
                pthread_mutex_unlock(&threadPool.taskQueue.mutex);
            }
        }
    }
}