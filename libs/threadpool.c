/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:16
 */
#define THREAD_POOL_DEBUG
#include "threadpool.h"
volatile int threads_keep_alive = 0;

thread_pool *
thread_pool_init(int nums)
{
    threads_keep_alive = 1;

    /* 创建线程池 */
    thread_pool *_pool = (thread_pool *)malloc(sizeof(thread_pool));
    if (_pool == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("thread pool make error\n");
#endif
        free(_pool);
        return NULL;
    }
    _pool->thread_alive_nums = 0;
    _pool->thread_working_nums = 0;
    _pool->threads = (thread **)malloc(sizeof(thread*) * nums);
    if (_pool->threads == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("thread pool make %d threads error\n",nums);
#endif
        free(_pool);
        return NULL;
    }
#ifdef THREAD_POOL_DEBUG
    printf("thread pool make success\n");
#endif
    /* 创建任务队列 */
    if (thread_job_queue_init(&_pool->job_queue) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("thread job queue make error\n");
#endif
        free(_pool);
    }
    return _pool;
}

static int
thread_job_queue_init(thread_job_queue **job_queue)
{
    (*job_queue) = (thread_job_queue *)malloc(sizeof(thread_job_queue));
    if ((*job_queue) == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("job queue malloc error\n");
#endif
    }
    (*job_queue)->len = 0;
    (*job_queue)->front = NULL;
    (*job_queue)->rear = NULL;
    /* 初始化互斥量 */
    if (pthread_mutex_init(&(*job_queue)->job_mutex,NULL) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("job queue pthread_mutex_init error\n");
#endif
        return -1;
    }
    return 0;
}

int
thread_job_queue_push(thread_job_queue *job_queue,thread_job *job)
{
    pthread_mutex_lock(&job_queue->job_mutex);
    if (job == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("job not found\n");
#endif
        return -1;
    }
    switch (job_queue->len) {
    case 0: /* 队列为空 */
            job_queue->front = job;
            job_queue->rear = job;
            break;
    default: /* 队列不为空 */
            job_queue->rear->next = job->next;
            job_queue->rear = job;
    }
    job_queue->len++;
    pthread_mutex_unlock(&job_queue->job_mutex);
    return 0;
}


thread_job * 
thread_job_pull(thread_job_queue *job_queue)
{
    thread_job *job = job_queue->front;
    if (job == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("job not found\n");
#endif
        return NULL;
    }
    switch (job_queue->len) {
    case 0: /* 队列为空 */
            break;
    case 1: /* 队列只存在一个元素 */
            job_queue->front = NULL;
            job_queue->rear = NULL;
            job_queue->len = 0;
            break;
    default: /* 队列元素大于1 */
            job_queue->front->next = job_queue->front;
            job_queue->len--;
    }
    return job;
}