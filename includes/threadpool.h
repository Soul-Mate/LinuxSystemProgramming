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


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
/* 任务节点结构 */
typedef struct thread_job {
    struct thread_job *next;    /* 下一个任务节点 */
    void* (*func)(void *arg);    /* 任务task */
    void *arg;                  /* 传递的参数 */
}thread_job;

/* 任务队列机构 */
typedef struct thread_job_queue {
    int len;                    /* 队列长度 */
    thread_job *front;          /* 上一个任务 */
    thread_job *rear;           /* 下一个任务 */
    pthread_mutex_t job_mutex;  /* 队列互斥量 */
}thread_job_queue;

/* 线程结构 */
typedef struct thread {
    int id;
    pthread_t pthread;
}thread;

/* 线程池结构 */
typedef struct thread_pool {
    thread **threads;                   /* 线程 */
    thread_job_queue *job_queue;        /* 任务队列 */
    volatile int thread_alive_nums;     /* 存活线程 */
    volatile int thread_working_nums;   /* 工作线程 */
}thread_pool;

thread_pool * thread_pool_init(int);

static int thread_job_queue_init(thread_job_queue **);
int thread_job_push(thread_job_queue *, thread_job *);
thread_job * thread_job_pull(thread_job_queue *);
static void thread_job_clear(thread_job_queue *);
static void thread_job_destroy(thread_job_queue *);
#endif //LINUX_SYSTEM_PROGRAMMING_THPOOL_H
