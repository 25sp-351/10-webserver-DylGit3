#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "response.h"

void parse_calc(const char *path, char *response_body);

void handle_request(int client_fd) {
    char buffer[4096];
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        return;

    buffer[bytes_read] = '\0';

    char method[16], path[256];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") != 0) {
        send_400(client_fd);
        return;
    }

    if (strncmp(path, "/static/", 8) == 0) {
        char file_path[512];
        snprintf(file_path, sizeof(file_path), ".%s", path);
        send_file(client_fd, file_path);
    } else if (strncmp(path, "/calc/", 6) == 0) {
        char result[128];
        parse_calc(path, result);
        send_200(client_fd, "text/html", result);
    } else {
        send_404(client_fd);
    }
}

void parse_calc(const char *path, char *response_body) {
    char op[8];
    int num1, num2, result;
    sscanf(path, "/calc/%[^/]/%d/%d", op, &num1, &num2);

    if (strcmp(op, "add") == 0)
        result = num1 + num2;
    else if (strcmp(op, "mul") == 0)
        result = num1 * num2;
    else if (strcmp(op, "div") == 0 && num2 != 0)
        result = num1 / num2;
    else {
        sprintf(response_body, "<h1>Invalid Operation</h1>");
        return;
    }

    sprintf(response_body, "<h1>Result: %d</h1>", result);
}
