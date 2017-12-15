/*
 * (C) XuYuanChang 2017
 *
 */

#include "main.h"
#include "sock.h"
#include <pthread.h>

int startup(u_short *);

void *accept_request(void *);

struct thread_arg {
    int sock_fd;
    struct sockaddr_in *addr;
};

int
main(int argc, const char *argv[])
{
    int                 sfd;
    int                 addr_len;
    u_short             port = 8090;
    pthread_t           tid;
    struct sockaddr_in  client_addr;
    struct thread_arg   arg; /* 线程函数所需参数 */
    arg.addr = &client_addr;
    if ((sfd = startup(&port)) < 0)
        err_msg("startup error");
    while(1) {
        addr_len = sizeof(client_addr);
        arg.sock_fd = accept(sfd,(struct sockaddr *)arg.addr,&addr_len);
        if (arg.sock_fd < 0)
            err_msg("accept error");
        /* 启动一个线程处理客户端连接请求 */
        if (pthread_create(&tid,NULL,accept_request,(void *)&arg) !=0 )
            err_msg("ptrhead_create error");
    }
    close(sfd);
    return 0;
}
int
startup(u_short *port)
{
    struct sockaddr_in server_addr;
    int    sfd,on = 1;
    memset(&server_addr,0,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(*port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if (sfd < 0)
        return sfd;
    if (setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on)) < 0)
        return -1;
    if (bind(sfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
        return -1;
    if (listen(sfd,3) < 0)
        return -1;
    return (sfd);
}

void
*accept_request(void *arg)
{
    char   msg[BUF_SIZE];
    int    total_read = 0;
    char   buf[BUF_SIZE];
    struct thread_arg *data;
    data = (struct thread_arg *)arg;
    sprintf(msg,"pthread 0x%lu accept: \n\t",(long)pthread_self());
    print_ip_and_port((const struct sockaddr *)data->addr,msg);
    total_read = read_line(data->sock_fd,buf,BUF_SIZE);
    if (write(data->sock_fd,str_upper(buf),total_read) != total_read)
        err_msg("write error");
    if (shutdown(data->sock_fd,SHUT_WR) < 0)
        err_msg("shutdown error");
    return NULL;
}
