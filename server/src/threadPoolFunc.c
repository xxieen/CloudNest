#include "../include/head.h"
#include "threadPool.h"

// 主线程初始化线程池
int pThreadPoolInit(threadPool_t *pThreadPool, int workerNum)
{
    pThreadPool->threadNum = workerNum;
    pThreadPool->tid = (pthread_t *)calloc(workerNum, sizeof(pthread_t));
    pThreadPool->taskQueue.pFront = NULL;
    pThreadPool->taskQueue.pRear = NULL;
    pThreadPool->taskQueue.size = 0;
    pthread_mutex_init(&pThreadPool->taskQueue.mutex, NULL);
    pthread_mutex_init(&pThreadPool->taskQueue.cond, NULL);
}

// 线程池中线程的工作内容
void *thread_function(void *arg)
{
    put("nothing now\n");
}

// 创建线程（工人）
int makeWorkers(threadPool_t *pThreadPool, int workerNum)
{
    for (int i = 0; i < workerNum; ++i)
    {
        pthread_create(&pThreadPool->tid[i], NULL, thread_function, (void *)pThreadPool);
    }
}
