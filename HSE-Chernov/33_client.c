#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* ./clnt localhost 80 
 * ./clnt google.com https 
 * ./clnt cs.hse.ru 80 */
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

    if (connect(fd, info->ai_addr, info->ai_addrlen) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }

    const char cmd[] = "GET /\r\n\r\n";
    write(fd, cmd, sizeof(cmd));
    char buf[1024];
    ssize_t z;
    while ((z = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, z);
    }
}