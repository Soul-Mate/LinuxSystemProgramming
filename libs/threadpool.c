/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:16
 */
#include "threadpool.h"

volatile int thread_keep_alive = 0;

/**
 * initialize job flag.
 * when flag equal zero, all threads  wait.
 * on success,returned zero. other,returned -1.
 * @param j
 * @return int
 */
int flag_init(struct job_flag **j)
{
    (*j) = (struct job_flag *)malloc(sizeof(struct job_flag));
    if ((*j) == NULL) {
        return -1;
    }
    (*j)->flag = 0;
    if (pthread_mutex_init(&(*j)->mutex,NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&(*j)->cond,NULL) != 0) {
        return -1;
    }
    return 0;
}

/**
 * wait job flag equal 1.
 * here, when job flag equal, all threads will rob task
 *
 * @param job_flag * j
 */
void flag_wait(struct job_flag *j)
{
    pthread_mutex_lock(&j->mutex);
    pthread_cond_signal(&j->cond);
    j->flag = 0;
    pthread_mutex_unlock(&j->mutex);
}

/**
 * notify all block threads
 *
 * @param job_flag j
 * @return int
 */
int flag_broadcast(struct job_flag *j)
{
    j->flag = 1;
    if (pthread_cond_broadcast(&j->cond) != 0) {
        return -1;
    }
    return 0;
}

int flag_signal(struct job_flag *j)
{
    j->flag = 1;
    if (pthread_cond_signal(&j->cond) != 0) {
        return -1;
    }
}

int job_queue_init(thread_job_queue **job_queue)
{
    (*job_queue) = (struct thread_job_queue *)malloc(sizeof(struct thread_job_queue));
    if ((*job_queue) == NULL) {
        return -1;
    }
    (*job_queue)->len = 0;
    (*job_queue)->front = NULL;
    (*job_queue)->rear = NULL;
    /* 初始化互斥量 */
    if (pthread_mutex_init(&(*job_queue)->job_mutex,NULL) != 0) {
        return -1;
    }
    /* 初始化任务标志 */
    if (flag_init(&(*job_queue)->has_job) != 0) {
        return -1;
    }
    return 0;
}

int job_queue_push(struct thread_job_queue *job_queue,struct thread_job *job)
{
    pthread_mutex_lock(&job_queue->job_mutex);
    switch (job_queue->len) {
        case 0: /* 队列为空 */
            job_queue->front = job;
            job_queue->rear = job;
            break;
        default: /* 队列不为空 */
            job_queue->rear->next = job;
            job_queue->rear = job;
    }
    job_queue->len++;
    /* 通知线程作业 */
    flag_signal(job_queue->has_job);
    pthread_mutex_unlock(&job_queue->job_mutex);
    return 0;
}

thread_job *job_queue_pull(thread_job_queue *job_queue)
{
    pthread_mutex_lock(&job_queue->job_mutex);
    thread_job *job;;
    switch (job_queue->len) {
        case 0: /* 队列为空 */
            job = NULL;
            break;
        case 1: /* 队列只存在一个元素 */
            job = job_queue->front;
            job_queue->front = NULL;
            job_queue->rear = NULL;
            job_queue->len = 0;
            break;
        default: /* 队列元素大于1 */
            job = job_queue->front;
            job_queue->front = job->next;
            job_queue->len--;
    }
    flag_signal(job_queue->has_job);
    pthread_mutex_unlock(&job_queue->job_mutex);
    return job;
}

void job_queue_clear(thread_job_queue *job_queue)
{
    while(job_queue->len) {
        free(job_queue_pull(job_queue));
    }
    job_queue->len = 0;
    job_queue->front = NULL;
    job_queue->rear = NULL;
}

void job_queue_destroy(thread_job_queue *job_queue)
{
    job_queue_clear(job_queue);
}

/**
 * initialize thread
 *
 * @param thread_pool * pool
 * @param thread ** t
 * @param const pthread_attr_t * attr
 * @param int i
 * @return
 */
int thread_init(struct thread_pool *pool, struct thread **t, const pthread_attr_t *attr, int i)
{
    (*t)  = (struct thread *)malloc(sizeof(struct thread));
    if ((*t) == NULL) {
        return -1;
    }
    (*t)->id = i;
    if (pthread_create(&(*t)->pthread,attr,thread_start,(void *)pool) != 0) {
        return -1;
    }
    return 0;
}

/**
 * function is runs after the thread is created
 *
 * @param void * arg
 * @return void *
 */
void *thread_start(void *arg)
{
    thread_pool *pool = (struct thread_pool *)arg;
    pthread_mutex_lock(&pool->pool_mutex);
    pool->thread_alive_nums++;
    pthread_mutex_unlock(&pool->pool_mutex);

    while(thread_keep_alive) {

        if (thread_keep_alive) {
            flag_wait(pool->job_queue->has_job);

            pthread_mutex_lock(&pool->pool_mutex);
            pool->thread_working_nums++;
            pthread_mutex_unlock(&pool->pool_mutex);

            thread_job *job = job_queue_pull(pool->job_queue);

            if (job) {
                job->func(job->arg);
                free(job);
            }

            pthread_mutex_lock(&pool->pool_mutex);
            pool->thread_working_nums--;
            if (!pool->thread_working_nums) {
                pthread_cond_signal(&pool->pool_cond);
            }
            pthread_mutex_unlock(&pool->pool_mutex);
        }
    }

    pthread_mutex_lock(&pool->pool_mutex);
    pool->thread_alive_nums--;
    pthread_mutex_unlock(&pool->pool_mutex);
    return NULL;
}

/**
 * initialize thread pool
 *
 * @param int nums
 * @return thread_pool *
 */
thread_pool *thread_pool_init(int nums)
{
    int i;
    pthread_attr_t attr;
    thread_keep_alive = 1;

    /* make a new thread pool */
    thread_pool *_pool = (thread_pool *)malloc(sizeof(thread_pool));
    if (_pool == NULL) {
        free(_pool);
        return NULL;
    }

    _pool->thread_alive_nums = 0;   /* alive threads */
    _pool->thread_working_nums = 0; /* working threads */


    /* make threads in threads pool */
    _pool->threads = (thread **)malloc(sizeof(thread*) * nums);
    if (_pool->threads == NULL) {
        free(_pool);
        return NULL;
    }

    /* initialize thread pool mutex */
    if (pthread_mutex_init(&_pool->pool_mutex,NULL) != 0) {
        free(_pool);
        return NULL;
    }

    /* initialize thread pool condition */
    if (pthread_cond_init(&_pool->pool_cond,NULL) != 0) {
        free(_pool);
        return NULL;
    }

    /* initialize thread attribute */
    if (pthread_attr_init(&attr) != 0) {
        free(_pool);
        return NULL;
    }
    /* set thread detach state attribute */
    if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) != 0) {
        free(_pool);
        return NULL;
    }
    /* set thread stack size equal PTHREAD_STACK_MIN(16384) * 2 */
    if (pthread_attr_setstacksize(&attr,PTHREAD_STACK_MIN * 2) != 0) {
        free(_pool);
        return NULL;
    }

    /* initialize job queue */
    if (job_queue_init(&_pool->job_queue) != 0) {
        job_queue_destroy(_pool->job_queue);
        free(_pool);
        return NULL;
    }

    /* create thread */
    for (i = 0; i < nums; i++) {
        if (thread_init(_pool,&_pool->threads[i],&attr,i) != 0) {
            job_queue_destroy(_pool->job_queue);
            free(_pool);
            return NULL;
        }
    }
    while (_pool->thread_alive_nums != nums); /* wait all threads create success */
    return _pool;
}

/**
 * add work to thread pool
 *
 * @param  thread_pool * pool
 * @param  void *(*func)(void *) func
 * @param  void *arg
 * @return int
 */
int thread_pool_add_work(struct thread_pool *pool, void*(*func)(void *), void *arg)
{
    thread_job *job = (struct thread_job *)malloc(sizeof(struct thread_job));
    if (job == NULL) {
        return -1;
    }
    job->arg = arg;
    job->func = func;
    job_queue_push(pool->job_queue,job);
    return 0;
}

/**
 * wait thread pool completed
 *
 * @param thread_pool * pool
 * @return
 */
int thread_pool_wait(thread_pool *pool)
{
    pthread_mutex_lock(&pool->pool_mutex);
    while(pool->thread_working_nums || pool->job_queue->len) {
        pthread_cond_wait(&pool->pool_cond,&pool->pool_mutex);
    }
    pthread_mutex_unlock(&pool->pool_mutex);
}
