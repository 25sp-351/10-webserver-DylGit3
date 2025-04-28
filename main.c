#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request.h"

#define DEFAULT_PORT 80
#define BUFFER_SIZE 4096

void *handle_client(void *arg);

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc == 3 && strcmp(argv[1], "-p") == 0)
        port = atoi(argv[2]);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd   = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    printf("Server listening on port %d\n", port);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient     = new_socket;
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    return 0;
}

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    free(arg);
    handle_request(client_fd);
    close(client_fd);
    return NULL;
}
