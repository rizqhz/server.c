#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static char buffer[1024];

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Invalid argument!\n");
        printf("Usage: %s <serv-addr> <serv-port>\n", argv[0]);
        printf("       %s 127.0.0.1 8080\n", argv[0]);
        return EINVAL;
    }

    const char* SERV_ADDR = argv[1];
    const int   SERV_PORT = atoi(argv[2]);

    /**
     * AF_INET => IPv4 Internet Protocol
     * SOCK_STREAM => Reliable connection-based byte stream (TCP)
     */
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1) {
        fprintf(stderr, "Failed to create an endpoint for communication!\n");
        return errno;
    }

    /**
     * sin_family => Server address family
     * sin_addr => Server address
     * sin_port => Server port
     */
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = inet_addr(SERV_ADDR),
        .sin_port = htons(SERV_PORT),
    };
    if (bind(tcp_fd, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Failed to bind an information to a socket!\n");
        close(tcp_fd);
        return errno;
    }

    /**
     * Listen for connections
     */
    if (listen(tcp_fd, 50) == -1)  {
        fprintf(stderr, "Failed to listen for connections on a socket!\n");
        close(tcp_fd);
        return errno;
    }

    /**
     * Accept a connection
     */
    struct sockaddr_in con_addr;
    socklen_t con_len = sizeof(con_addr);
    for (int i = 0; i < 5; ++i) {
        int con_fd = accept(tcp_fd, (struct sockaddr*)&con_addr, &con_len);
        if (con_fd == -1) {
            fprintf(stderr, "Failed to accept a connection on a socket!\n");
            break;
        }

        recv(con_fd, buffer, sizeof(buffer), 0);
        fprintf(stdout, "%s: %s\n", inet_ntoa(con_addr.sin_addr), buffer);

        strcpy(buffer, "Hello from server!");
        send(con_fd, buffer, sizeof(buffer), 0);

        close(con_fd);
    }

    close(tcp_fd);
    return 0;
}
