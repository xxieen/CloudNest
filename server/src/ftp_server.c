#include "../include/head.h"
#include "threadPool.h"
#include "serverTransferFile.h"

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
    struct epoll_event readyArr[5];

    int netFd = 0;

    // 不断的等待epoll
    while (1)
    {

        int readyNum = epoll_wait(epfd, readyArr, 2, -1);

        for (int i = 0; i < readyNum; ++i)
        {
            // 是来新客户端了吗
            if (readyArr[i].data.fd == sockFd)
            {

                netFd = accept(sockFd, NULL, NULL);
                // 拿到了netFd就放进监听，注意多个netFd情况
                epollAdd(netFd, epfd);
            }
            else if (0)
            {
                printf("如果还有别的需要监听"); // 优雅退出
            }
            else
            { // 别的都不是肯定就是已连接的netFd了

                int type = 0;

                char cmd[100] = {0};
                recv(readyArr[i].data.fd, cmd, sizeof(cmd), 0);
                // 解析命令
                // 函数：找到命令相对应的类型，swich返回一个int  SEND_FILE  or RECIVE_FILE
                // 用type接住返回的类型
                // char* cmdArr[] = split(cmd);
                // if(cmdArr[0] == "download"){
                //     type = SEND_FILE;
                // }

                // 传一个task

                pthread_mutex_lock(&threadPool.taskQueue.mutex);
                taskEnqueue(&threadPool.taskQueue, readyArr[i].data.fd, type);

                pthread_cond_signal(&threadPool.taskQueue.cond);

                pthread_mutex_unlock(&threadPool.taskQueue.mutex);
            }
        }
    }
}
