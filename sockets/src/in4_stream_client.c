/*
 * (C) XuYuanChang 2017
 * 一个简单的转换大小写客户端 基于TCP 支持IPV4
 */

#include "main.h"
#include "sock.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
        err_msg("Usage ./in4_stream_client input");
    const char *SERVER_IP = "127.0.0.1";
    const int  SERVER_PORT = 8090;
    int cfd,total_read;
    char buf[BUF_SIZE];
    struct sockaddr_in sv_addr,cl_addr;
    bzero(&sv_addr,sizeof(struct sockaddr_in));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&sv_addr.sin_addr.s_addr);
    if((cfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        err_msg("socket error");
    if (connect(cfd,(const struct sockaddr *)&sv_addr,sizeof(struct sockaddr_in)) < 0)
        err_msg("connect error");
    if (write(cfd,argv[1],strlen(argv[1])) < 0)
        err_msg("write error");
    if (write(cfd,"\n",1) != 1)
        err_msg("write error");
    total_read = read_line(cfd,buf,BUF_SIZE);
    if (total_read == -1)
        err_msg("read_line error");
    printf("%s\n",buf);
    while (1);
    return 0;
}