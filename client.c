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
    /**
     * AF_INET => IPv4 Internet Protocol
     * SOCK_STREAM => Reliable connection-based byte stream (TCP)
     */
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1) {
        fprintf(stderr, "Failed to create an endpoint for communication!\n");
        return errno;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(8080),
    };
    socklen_t len = sizeof(addr);
    if (connect(tcp_fd, (const struct sockaddr*)&addr, len) == -1) {
        fprintf(stderr, "Failed to initiate a connection on a socket!\n");
        close(tcp_fd);
        return errno;
    }

    strcpy(buffer, "Hello from client!");
    send(tcp_fd, buffer, sizeof(buffer), 0);
    
    recv(tcp_fd, buffer, sizeof(buffer), 0);
    fprintf(stdout, "%s: %s\n", inet_ntoa(addr.sin_addr), buffer);

    close(tcp_fd);
    return 0;
}
