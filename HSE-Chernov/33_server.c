#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

/* gcc 33_server.c -O2 -Wall -Werror -std=gnu11 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64
 * ./serv 0.0.0.0 9999 (shell 1)
 * nc localhost 9999 (shell 2)
 * netstat -ant 
 * ls -l /proc/<serv_pid>/fd */
int main(int argc, char **argv)
{
    const char *host = argv[1];
    const char *service = argv[2];

    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *info = NULL;

    int err = getaddrinfo(host, service, &hints, &info);
    if (err) {
        fprintf(stderr, "error: %s\n", gai_strerror(err));
        exit(1);
    }

    int fd = socket(PF_INET, SOCK_STREAM, 0);

    if (bind(fd, info->ai_addr, info->ai_addrlen) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);        
    }

    listen(fd, 5);

    /* несколько клиентов:
     * 1) свой процесс для каждого клиента
     * 2) select/epoll 
     * 3) отдельная нить для каждого клиента */

    while (1) {

        /* отец: подключение новых клиентов */
        struct sockaddr_in new_addr;
        socklen_t new_len = sizeof(struct sockaddr_in);
        int afd = accept(fd, &new_addr, &new_len);

        /* сыновья: обработка */
        if (!fork()) {
            close(fd);
            printf("connected: %s %d\n", inet_ntoa(new_addr.sin_addr),
                ntohs(new_addr.sin_port)); // из сетевого порядка байт в локальный

            char c;
            while (read(afd, &c, sizeof(c)) > 0) {
                printf("%d %c\n", getpid(), c);
                fflush(stdout);
            }
            _exit(0);
        }

        close(afd);
        while (waitpid(-1, NULL, WNOHANG) > 0) {}
    }
}