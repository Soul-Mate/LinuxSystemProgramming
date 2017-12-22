/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:16
 */
#define THREAD_POOL_DEBUG

#include "threadpool.h"

volatile int threads_keep_alive = 0;

/**
 * 初始化线程池
 * @param int nums
 * @return thread_pool *
 */
 thread_pool *
thread_pool_init(int nums)
{
    int i;
    pthread_attr_t attr;
    threads_keep_alive = 1;

    /* 初始化线程池 */
    thread_pool *_pool = (thread_pool *)malloc(sizeof(thread_pool));
    if (_pool == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: thread pool make error\n");
#endif
        free(_pool);
        return NULL;
    }
    _pool->thread_alive_nums = 0;
    _pool->thread_working_nums = 0;
    _pool->threads = (thread **)malloc(sizeof(thread*) * nums);
    if (_pool->threads == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: thread pool make threads error\n");
#endif
        free(_pool);
        return NULL;
    }
#ifdef THREAD_POOL_DEBUG
    printf("Debug Success: thread pool make success\n");
#endif
    /* 初始化任务队列 */
    if (thread_job_queue_init(&_pool->job_queue) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: thread job queue make error\n");
#endif
        thread_job_queue_destroy(_pool->job_queue);
        free(_pool);
        return NULL;
    }
    /* 初始化线程属性 */
    if (pthread_attr_init(&attr) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: thread init attr error\n");
#endif
        thread_job_queue_destroy(_pool->job_queue);
        free(_pool);
        return NULL;
    }
    /* 设置线程分离属性 */
    if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("set thread detach state attr error\n");
#endif
        thread_job_queue_destroy(_pool->job_queue);
        free(_pool);
        return NULL;
    }
    /* 设置线程栈属性 */
    if (pthread_attr_setstacksize(&attr,16384 * 2) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("set thread stack size attr error\n");
#endif
        thread_job_queue_destroy(_pool->job_queue);
        free(_pool);
        return NULL;
    }
    /* 添加线程到线程池 */
    for (i = 0; i < nums; i++) {
        if (thread_init(_pool,&_pool->threads[i],&attr,i) != 0) {
#ifdef THREAD_POOL_DEBUG
            printf("thread pool add thread error\n");
#endif
            thread_job_queue_destroy(_pool->job_queue);
            free(_pool);
            return NULL;
        }
    }
    while (_pool->thread_alive_nums != nums); /* 等待所有线程创建完毕 */
    return _pool;
}

 int
thread_pool_add_work(thread_pool *pool, void*(*func)(void *), void *arg)
{
    thread_job *job = (thread_job *)malloc(sizeof(thread_job));
    job->arg = arg;
    job->func = func;
    job->next = NULL;
    thread_job_queue_push(pool->job_queue,job);
    return 0;
}

 int
thread_pool_wait(thread_pool *pool)
{
    pthread_mutex_lock(&pool->pool_mutex);
    while(pool->thread_working_nums || pool->job_queue->len) {
        pthread_cond_wait(&pool->pool_cond,&pool->pool_mutex);
    }
    pthread_mutex_unlock(&pool->pool_mutex);
}


/**
 * 初始化线程
 * @param thread_pool * pool
 * @param thread * thread
 * @param const pthread_attr_t * attr
 * @return
 */
static int
thread_init(struct thread_pool *pool, thread **thread, const pthread_attr_t *attr, int i)
{
    (*thread) = (struct thread *)malloc(sizeof(struct thread));
    if (*thread == NULL) {
        return -1;
    }
    (*thread)->id = i;
    /* 创建线程 */
    if (pthread_create(&(*thread)->pthread,attr,thread_start,(void *)pool) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: create thread %d error\n",i);
#endif
        return -1;
    }
    return 0;
}

void *
thread_start(void *arg)
{
    thread_pool *pool = (thread_pool *)arg;
    pthread_mutex_lock(&pool->pool_mutex);
    pool->thread_alive_nums++;
    pthread_mutex_unlock(&pool->pool_mutex);
    while(threads_keep_alive) {
        if (threads_keep_alive) {
            pthread_mutex_lock(&pool->pool_mutex);
            pool->thread_working_nums++;
            pthread_mutex_unlock(&pool->pool_mutex);
            /* 阻塞等待任务发生 */
            job_flag_wait(pool->job_queue->has_job);
            thread_job * job = thread_job_queue_pull(pool->job_queue);
            if (job) {
                /* 运行任务 */
                job->func(job->arg);
                free(job);
            }
            pthread_mutex_lock(&pool->pool_mutex);
            pool->thread_working_nums--;
            /* 线程池任务运行结束 */
            if (pool->thread_working_nums == 0) {
                pthread_cond_signal(&pool->pool_cond);
            }
            pthread_mutex_unlock(&pool->pool_mutex);
        }
    }
    return NULL;
}


static int
thread_job_queue_init(thread_job_queue **job_queue)
{
    (*job_queue) = (struct thread_job_queue *)malloc(sizeof(struct thread_job_queue));
    if ((*job_queue) == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: job queue malloc error\n");
#endif
        return -1;
    }
    (*job_queue)->len = 0;
    (*job_queue)->front = NULL;
    (*job_queue)->rear = NULL;
    /* 初始化互斥量 */
    if (pthread_mutex_init(&(*job_queue)->job_mutex,NULL) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: job queue pthread_mutex_init error\n");
#endif
        return -1;
    }
    /* 初始化任务标志 */
    if (job_flag_init(&(*job_queue)->has_job) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: job queue init job flag error\n");
#endif
        return -1;
    }
    return 0;
}

static int
thread_job_queue_push(thread_job_queue *job_queue,thread_job *job)
{
    if (job == NULL) {
#ifdef THREAD_POOL_DEBUG
        printf("job not found\n");
#endif
        return -1;
    }
    pthread_mutex_lock(&job_queue->job_mutex);
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
    /* 通知线程作业 */
    job_flag_signal(job_queue->has_job);
    return 0;
}

static thread_job *
thread_job_queue_pull(thread_job_queue *job_queue)
{
    pthread_mutex_lock(&job_queue->job_mutex);
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
    pthread_mutex_unlock(&job_queue->job_mutex);
    return job;
}

static void
thread_job_queue_clear(thread_job_queue *job_queue)
{
    while(job_queue->len) {
        free(thread_job_queue_pull(job_queue));
    }
    job_queue->len = 0;
    job_queue->front = NULL;
    job_queue->rear = NULL;
}

static void
thread_job_queue_destroy(thread_job_queue *job_queue)
{
    thread_job_queue_clear(job_queue);
}

static int
job_flag_init(job_flag **flag)
{
    (*flag) = (struct job_flag *)malloc(sizeof(struct job_flag));
    if ((*flag) == NULL) {
        return -1;
    }
    (*flag)->flag = 0;
    if (pthread_mutex_init(&(*flag)->mutex,NULL) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: job flag init mutex error\n");
#endif
        return -1;
    }
    if (pthread_cond_init(&(*flag)->cond,NULL) != 0) {
#ifdef THREAD_POOL_DEBUG
        printf("Debug Error: job flag init cond error\n");
#endif
        return -1;
    }
    return 0;
}
static int
job_flag_wait(job_flag *flag)
{
    pthread_mutex_lock(&flag->mutex);
    while(flag->flag == 0) {
        if (pthread_cond_wait(&flag->cond,&flag->mutex) != 0) {
#ifdef THREAD_POOL_DEBUG
            printf("Error: job flag wait cond error\n");
#endif
            return -1;
        }
    }
    flag->flag = 0;
    pthread_mutex_unlock(&flag->mutex);
    return 0;
}

static int
job_flag_signal(job_flag *flag)
{
    if (pthread_mutex_lock(&flag->mutex) != 0) {
        return -1;
    }
    flag->flag = 1;
    if (pthread_cond_signal(&flag->cond) != 0) {
        return -1;
    }
    if (pthread_mutex_unlock(&flag->mutex) != 0) {
        return -1;
    }
    return 0;
}

static int
job_flag_broadcast(job_flag * flag)
{
    if (pthread_mutex_lock(&flag->mutex) != 0) {
        return -1;
    }
    flag->flag = 1;
    if (pthread_cond_broadcast(&flag->cond) != 0) {
        return -1;
    }
    if (pthread_mutex_unlock(&flag->mutex) != 0) {
        return -1;
    }
    return 0;
}

static int
job_flag_reset(job_flag *flag)
{
    if (job_flag_init(flag) != 0) {
        return -1;
    }
    return 0;
}