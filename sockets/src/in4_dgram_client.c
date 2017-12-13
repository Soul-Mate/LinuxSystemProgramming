#include "main.h"
#include "sock.h"

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        printf("usage error\n");
        exit(0);
    }
    const char         *SERVER_IP = "127.0.0.1";
    const              u_short SERVER_PORT = 4000;
    char               *buf;
    int                cl_fd,read_num,addr_len;
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr.s_addr) <= 0)
        err_msg("inet_pton error");
    cl_fd =socket(AF_INET,SOCK_DGRAM,0);
    if (cl_fd < 0)
        err_msg("socket");
    if (connect(cl_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) < -1 )
        err_msg("connect error");
    if (sendto(cl_fd, argv[1], 1024, 0 ,
               (struct sockaddr *)&server_addr,
               sizeof(struct sockaddr_in)) == -1
            )
        err_msg("sendto error");
    addr_len = sizeof(struct sockaddr);
    buf = malloc(1024);
    read_num = recvfrom(cl_fd, buf, 1024,0,(struct sockaddr *)&server_addr,&addr_len);
    print_ip_and_port((const struct sockaddr *)&server_addr,"connect success");
    free(buf);
}