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
    int sfd;
    struct sockaddr_in serv_addr,cl_addr;
    bzero(&serv_addr,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&serv_addr.sin_addr.s_addr);
    if((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        err_msg("socket error");
    if (connect(sfd,(const struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0)
        err_msg("connect error");
    return 0;
}