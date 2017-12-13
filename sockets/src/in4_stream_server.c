/*
 * (C) XuYuanChang 2017
 * 一个简单的转换大小写服务器 基于TCP 支持IPV4
 */
#include "main.h"
#include "sock.h"

int main(int argc, const char *argv[])
{
    const  int SERVER_PORT = 8090;
    int    sfd,cfd,addr_len,total_read;
    char   buf[BUF_SIZE];
    struct sockaddr_in server_addr,client_addr;
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
        print_ip_and_port((const struct sockaddr *)&client_addr,"connect success");
        total_read = read_line(cfd,buf,BUF_SIZE);
        if (total_read == -1)
            err_msg("read_line error");
        strcpy(buf,str_upper(buf));
        if (write(cfd,&buf,strlen(buf)) != total_read)
            err_msg("write error");
    }
    return 0;
}