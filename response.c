#include "response.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void send_200(int client_fd, const char *content_type, const char *body) {
    char header[512];
    int content_length = strlen(body);

    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n\r\n",
             content_type, content_length);

    write(client_fd, header, strlen(header));
    write(client_fd, body, content_length);
}

void send_file(int client_fd, const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        send_404(client_fd);
        return;
    }

    struct stat st;
    fstat(fd, &st);
    int content_length = st.st_size;

    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: application/octet-stream\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n\r\n",
             content_length);

    write(client_fd, header, strlen(header));

    char file_buffer[1024];
    int bytes_read;
    while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0)
        write(client_fd, file_buffer, bytes_read);

    close(fd);
}

void send_404(int client_fd) {
    const char *body = "<h1>404 Not Found</h1>";
    send_200(client_fd, "text/html", body);
}

void send_400(int client_fd) {
    const char *body = "<h1>400 Bad Request</h1>";
    send_200(client_fd, "text/html", body);
}
