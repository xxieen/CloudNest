#include "../include/head.h"
#include "threadPool.h"
#include "serverTransferFile.h"

// 主线程初始化线程池
int pThreadPoolInit(threadPool_t *pThreadPool, int workerNum)
{
    pThreadPool->threadNum = workerNum;
    pThreadPool->tid = (pthread_t *)calloc(workerNum, sizeof(pthread_t));
    pThreadPool->taskQueue.pFront = NULL;
    pThreadPool->taskQueue.pRear = NULL;
    pThreadPool->taskQueue.size = 0;
    pthread_mutex_init(&pThreadPool->taskQueue.mutex, NULL);
    pthread_cond_init(&pThreadPool->taskQueue.cond, NULL);
    return 0;
}

// 线程池中线程的工作内容
void *thread_function(void *arg)
{
    while (1)
    {
        threadPool_t *pThreadPool = (threadPool_t *)arg;
        int netFd;

        pthread_mutex_lock(&pThreadPool->taskQueue.mutex);
        // 等待netFd
        puts("workers wait for task");

        while (pThreadPool->taskQueue.size == 0)
        {

            pthread_cond_wait(&pThreadPool->taskQueue.cond, &pThreadPool->taskQueue.mutex);
        }

        // 获取netFd和type（出队）
        netFd = pThreadPool->taskQueue.pFront->netFd;
        char **command = pThreadPool->taskQueue.pFront->command;
        taskDequeue(&pThreadPool->taskQueue);
        pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);

        puts("send_file begin!");
        printf("command[0] = %s", command[0]);
        printf("cc:%d", strcmp(command[0], "download"));
        if (strcmp(command[0], "download") == 0)
        {
            puts("进来了嘛");
            serverSendFile(netFd, command[1]);
            puts("send_file finish!");
        }
        if (strcmp(command[0], "upload") == 0)
        {
            serverRecvFile(netFd);
        }
    }

    return NULL;
}

// 创建线程（工人）
int makeWorkers(threadPool_t *pThreadPool, int workerNum)
{
    for (int i = 0; i < workerNum; ++i)
    {

        pthread_create(&pThreadPool->tid[i], NULL, thread_function, (void *)pThreadPool);
    }
    return 0;
}
