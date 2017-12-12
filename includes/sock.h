#ifndef LINUX_SYSTEM_PROGRAMMING_SOCK_H
#define LINUX_SYSTEM_PROGRAMMING_SOCK_H

#include <arpa/inet.h>
#include <sys/socket.h>
void    print_ip_and_port(const struct sockaddr *, char *);
int     readLine(int, void *, size_t);
#endif //LINUX_SYSTEM_PROGRAMMING_SOCK_H
