#ifndef LINUX_SYSTEM_PROGRAMMING_SOCK_H
#define LINUX_SYSTEM_PROGRAMMING_SOCK_H

#define BUF_SIZE 500
#include <arpa/inet.h>
#include <sys/socket.h>
void    print_ip_and_port(const struct sockaddr *, char *);
#endif //LINUX_SYSTEM_PROGRAMMING_SOCK_H
