#ifndef RESPONSE_H
#define RESPONSE_H

void send_200(int client_fd, const char *content_type, const char *body);
void send_file(int client_fd, const char *file_path);
void send_404(int client_fd);
void send_400(int client_fd);

#endif
