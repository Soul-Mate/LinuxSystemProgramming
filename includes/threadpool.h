/*
 * (C) XuYuanChang 2017
 * 2017/12/20 17:23
 * 线程池头文件
 */

#ifndef LINUX_SYSTEM_PROGRAMMING_THPOOL_H
#define LINUX_SYSTEM_PROGRAMMING_THPOOL_H

#ifdef THREAD_POOL_DEBUG
#define THREAD_POOL_DEBUG 1
#else
#define THREAD_POOL_DEBUG 0
#endif


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 任务标识结构 */
typedef struct job_flag {
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}job_flag;

/* 任务节点结构 */
typedef struct thread_job {
    struct thread_job *next;    /* 下一个任务节点 */
    void* (*func)(void *arg);    /* 任务task */
    void *arg;                  /* 传递的参数 */
} thread_job;

/* 任务队列机构 */
typedef struct thread_job_queue {
    int len;                    /* 队列长度 */
    thread_job *front;          /* 上一个任务 */
    thread_job *rear;           /* 下一个任务 */
    pthread_mutex_t job_mutex;  /* 队列互斥量 */
    job_flag *has_job;          /* 任务标志 */
} thread_job_queue;

/* 线程结构 */
typedef struct thread {
    int id;
    pthread_t pthread;
    struct thread_pool *pool;
} thread;

/* 线程池结构 */
typedef struct thread_pool {
    thread **threads;                   /* 线程 */
    thread_job_queue *job_queue;        /* 任务队列 */
    volatile int thread_alive_nums;     /* 存活线程 */
    volatile int thread_working_nums;   /* 工作线程 */
    pthread_mutex_t pool_mutex;         /* 线程池互斥锁 */
    pthread_cond_t pool_cond;           /* 线程池信号了 */
} thread_pool;

thread_pool * thread_pool_init(int);
int thread_pool_add_work(thread_pool *, void*(*func)(void *),void*);
int thread_pool_wait(thread_pool *);

int get_thread_pool_work(thread_pool *);

int thread_init(thread_pool *, thread **, const pthread_attr_t *,int);
void *thread_start(void *);

static int thread_job_queue_init(thread_job_queue **);
static int thread_job_queue_push(thread_job_queue *, thread_job *);
static thread_job * thread_job_queue_pull(thread_job_queue *);
static void thread_job_queue_clear(thread_job_queue *);
static void thread_job_queue_destroy(thread_job_queue *);

static int job_flag_init(job_flag **);
static int job_flag_wait(job_flag *);
static int job_flag_signal(job_flag *);
static int job_flag_broadcast(job_flag *);
static int job_flag_reset(job_flag *);
#endif //LINUX_SYSTEM_PROGRAMMING_THPOOL_H
