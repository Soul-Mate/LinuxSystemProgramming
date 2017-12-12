/*
 * (C) XuYuanChang 2017
 * sockets章节用到的各函数
 */
#include "main.h"
#include "sock.h"
/**
 * @desc 打印ip和端口号
 * @param sock
 */
void print_ip_and_port(const struct sockaddr * sock,char *s)
{
    char *ip;
    const int IP_SIZE = 128;
    ip = malloc(IP_SIZE);
    struct sockaddr_in *client;
    client = (struct sockaddr_in *)sock;
    printf("%s ip: %s, port: %d\n",
           s,
           inet_ntop(AF_INET,((const char *)&(client->sin_addr.s_addr)),ip,IP_SIZE),
           ntohs(client->sin_port)
    );
}
