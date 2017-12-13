#include "main.h"
#include "sock.h"

int main(int argc, const char *argv[])
{
    const   u_short SERVER_PORT = 4000;
    int     sfd;
    int     read_num;
    int     addr_len;
    char    *buf;
    struct  sockaddr_in sv_addr,cl_addr;
    bzero(&sv_addr, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port = htons(SERVER_PORT);
    sfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sfd < 0)
        err_msg("socket error");
    if (bind(sfd, (struct sockaddr *)&sv_addr, sizeof(struct sockaddr_in)) == -1)
        err_msg("bind error");
    for(;;) {
        addr_len = sizeof(struct sockaddr_in);
        buf = malloc(BUF_SIZE);
        read_num = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&cl_addr,&addr_len);
        if (read_num == -1)
            /* 如果系统调用被中断 重新读取 */
            if (errno == EINTR)
                continue;
            else
                err_msg("recvfrom error");
        else {
            print_ip_and_port((const struct sockaddr *)&cl_addr,"connect success");
            if (sendto(sfd,str_upper(buf),BUF_SIZE,0,
                        (struct sockaddr *)&cl_addr,
                        sizeof(struct sockaddr)) <= 0)
                err_msg("sendto error");
            free(buf);
        }
    }
}

