/*
 * (C) XuYuanChang 2017
 * 一个简单的转换大小写并发服务器 基于TCP 支持IPV4 使用多进程模型
 */
#include "main.h"
#include "sock.h"
#include <sys/wait.h>

void handler_request(int, const struct sockaddr *);

void handler_signal(int);

int main(int argc, const char *argv[])
{
    const  int SERVER_PORT = 8090;
    int    sfd,cfd,addr_len;
    struct sockaddr_in server_addr,client_addr;
    struct sigaction sa;
    /* 设置SIGCHILD信号捕捉函数 */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler_signal;
    if (sigaction(SIGCHLD,&sa,NULL) == -1)
        err_msg("sigaction error");
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if (sfd < 0)
        err_msg("socket error");
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    if (bind(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) < 0 )
        err_msg("bind error");
    if (listen(sfd,128) < 0)
        err_msg("listen error");
    addr_len = sizeof(struct sockaddr_in);
    for(;;) {
        if ((cfd = accept(sfd,(struct sockaddr *)&client_addr,&addr_len)) < 0)
            err_msg("accept error");
        switch (fork()) {
            case -1:
                err_msg("fork error");
                close(cfd);
                break;
            case 0:
                close(sfd);
                handler_request(cfd,(const struct sockaddr *)&client_addr);
                _exit(EXIT_SUCCESS);
            default:
                close(cfd);
                break;
        }
    }
    return 0;
}

void
handler_signal(int signo)
{
    int save_errno;
    save_errno = errno;
    /* 回收退出的进程 */
    while (waitpid(-1,NULL,WNOHANG) > 0)
        continue;
    errno = save_errno;
}

void
handler_request(int fd, const struct sockaddr * addr)
{
    int  total_read;
    char buf[BUF_SIZE];
    print_ip_and_port(addr,"connect success");
    total_read = read_line(fd,buf,BUF_SIZE);
    if (total_read == -1)
        err_msg("read_line error");
    strcpy(buf,str_upper(buf));
    if (write(fd,&buf,total_read) != total_read)
        err_msg("write error");
    if (close(fd) < 0)
        err_msg("close error");
}