#include "readline.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void errExit(char *msg);

void printIpAndPort(const struct sockaddr *);

int main(int argc, const char *argv[])
{
    const   size_t  BUF_SIZE = 1024;
    const   u_short SERVER_PORT = 4000;
    int     serverFd,clientFd;
    int     readNum;
    int     addressLen;
    char    *buf;
    struct  sockaddr_in serverAddress,clientAddress;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(SERVER_PORT);
    serverFd = socket(AF_INET,SOCK_DGRAM,0);
    if (serverFd < 0)
        errExit("socket error");
    if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == -1)
        errExit("bind error");
    buf = malloc(BUF_SIZE);
    for(;;) {
        addressLen = sizeof(struct sockaddr_in);
        readNum = recvfrom(serverFd,buf,BUF_SIZE,0,(struct sockaddr *)&clientAddress,&addressLen);
        if (readNum == -1)
            /* 如果系统调用被中断 重新读取 */
            if (errno == EINTR)
                continue;
            else
                errExit("recvfrom error");
        printIpAndPort((const struct sockaddr *)&clientAddress);
    }
}
/**
 * @desc 错误退出
 * @param msg
 */
void errExit(char *msg) {
    perror(msg);
    exit(0);
}

void printIpAndPort(const struct sockaddr * sock)
{
    char *ip; /* ip地址 */
    const int IP_SIZE = 128;
    ip = malloc(IP_SIZE);
    struct sockaddr_in *client;
    client = (struct sockaddr_in *)sock;
    printf("Connect client ip: %s, port: %d\n",
           inet_ntop(AF_INET,((const char *)&(client->sin_addr.s_addr)),ip,IP_SIZE),
           ntohs(client->sin_port)
    );
}
