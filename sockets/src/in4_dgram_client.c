#include "readline.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
void errExit(char *msg);

void printIpAndPort(const struct sockaddr * sock);

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        printf("usage error\n");
        exit(0);
    }
    const char         *SERVER_IP = "127.0.0.1";
    const              u_short SERVER_PORT = 4000;
    char               *buf;
    int                clientFd,readNum,addrLen;
    struct sockaddr_in serverAddress;
    bzero(&serverAddress,sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET,SERVER_IP,&serverAddress.sin_addr.s_addr) <= 0)
        errExit("inet_pton error");
    clientFd =socket(AF_INET,SOCK_DGRAM,0);
    if (clientFd < 0)
        errExit("socket");
    if (connect(clientFd,(struct sockaddr *)&serverAddress,sizeof(struct sockaddr)) < -1 )
        errExit("connect error");
    if (sendto(clientFd, argv[1], 1024, 0 ,
               (struct sockaddr *)&serverAddress,
               sizeof(struct sockaddr_in)) == -1
            )
        errExit("sendto error");
    addrLen = sizeof(struct sockaddr);
    buf = malloc(1024);
    readNum = recvfrom(clientFd, buf, 1024,0,(struct sockaddr *)&serverAddress,&addrLen);
    printIpAndPort((const struct sockaddr *)&serverAddress);
    free(buf);
}
/**
 * @desc 错误退出
 * @param msg
 */
void errExit(char *msg) {
    perror(msg);
    exit(0);
}

/**
 * @desc 打印ip和端口号
 * @param sock
 */
void printIpAndPort(const struct sockaddr * sock)
{
    char *ip; /* ip地址 */
    const int IP_SIZE = 128;
    ip = malloc(IP_SIZE);
    struct sockaddr_in *client;
    client = (struct sockaddr_in *)sock;
    printf("recv success:\n\tserver ip: %s, port: %d\n",
           inet_ntop(AF_INET,((const char *)&(client->sin_addr.s_addr)),ip,IP_SIZE),
           ntohs(client->sin_port)
    );
}