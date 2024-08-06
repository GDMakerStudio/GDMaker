#include "http_server.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <arm-linux-gnueabihf/sys/socket.h>
#include <arm-linux-gnueabihf/sys/types.h>


#ifndef(WINDOWS)
// NOTE : ON WINDOWS YOU NEED THIS OR INSTALL PACKAGES.PY FILE TO INSTALL 
// IN WINDOWS
// #include <winsock2.h>
// #include <ws2tcpip.h>

#define BUFFER_SIZE 1024

bool http_server_start(HttpServer* self, int port) {
    struct sockaddr_in server_addr;

    self->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (self->server_fd == -1) {
        perror("socket creation failed");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(self->server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("socket bind failed");
        close(self->server_fd);
        return false;
    }

    if (listen(self->server_fd, 5) != 0) {
        perror("listen failed");
        close(self->server_fd);
        return false;
    }

    self->running = true;
    return true;
}

void http_server_handle_request(HttpServer* self) {
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);

    int conn_fd = accept(self->server_fd, (struct sockaddr *)&client_addr, &len);
    if (conn_fd < 0) {
        perror("server accept failed");
        return;
    }

    char buffer[BUFFER_SIZE];
    int n = read(conn_fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read failed");
        close(conn_fd);
        return;
    }
    buffer[n] = '\0';

    printf("Request:\n%s\n", buffer);

    // Sending a simple response
    const char *response = "HTTP/1.1 200 OK\nContent-Length: 13\n\nHello, world!";
    write(conn_fd, response, strlen(response));
    close(conn_fd);
}

void http_server_stop(HttpServer* self) {
    self->running = false;
    close(self->server_fd);
}

size_t http_server_get_response_body_length(HttpServer* self) {
    return self->response_body_length;
}

bool http_server_connect_to_host(HttpServer* self, const char* host, int port) {
    struct sockaddr_in server_addr;

    self->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (self->server_fd == -1) {
        perror("socket creation failed");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    if (connect(self->server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("connect failed");
        close(self->server_fd);
        return false;
    }

    return true;
}

void http_server_close(HttpServer* self) {
    close(self->server_fd);
    self->server_fd = -1;
}

int http_server_get_response_code(HttpServer* self) {
    return self->response_code;
}

char* http_server_get_response_headers(HttpServer* self) {
    return self->response_headers;
}

const char* http_server_get_status(HttpServer* self) {
    return self->running ? "running" : "stopped";
}

char* http_server_query_string_from_dict(HttpServer* self, const char** dict) {
    size_t total_length = 0;
    for (size_t i = 0; dict[i] && dict[i + 1]; i += 2) {
        total_length += strlen(dict[i]) + strlen(dict[i + 1]) + 2; // key=value&
    }
    total_length++; // For the null terminator

    char* query_string = (char*)malloc(total_length);
    if (!query_string) {
        perror("malloc failed");
        return NULL;
    }

    query_string[0] = '\0';
    for (size_t i = 0; dict[i] && dict[i + 1]; i += 2) {
        strcat(query_string, dict[i]);
        strcat(query_string, "=");
        strcat(query_string, dict[i + 1]);
        if (dict[i + 2]) {
            strcat(query_string, "&");
        }
    }

    return query_string;
}

void http_server_poll(HttpServer* self) {
    while (self->running) {
        http_server_handle_request(self);
    }
}

HttpServer* http_server_new() {
    HttpServer* server = (HttpServer*)malloc(sizeof(HttpServer));
    server->start = http_server_start;
    server->handle_request = http_server_handle_request;
    server->stop = http_server_stop;
    server->get_response_body_length = http_server_get_response_body_length;
    server->connect_to_host = http_server_connect_to_host;
    server->close = http_server_close;
    server->get_response_code = http_server_get_response_code;
    server->get_response_headers = http_server_get_response_headers;
    server->get_status = http_server_get_status;
    server->query_string_from_dict = http_server_query_string_from_dict;
    server->poll = http_server_poll;
    server->server_fd = -1;
    server->running = false;
    server->response_body_length = 0;
    server->response_code = 200;
    server->response_headers = NULL;
    return server;
}

void http_server_free(HttpServer* server) {
    if (server) {
        if (server->response_headers) {
            free(server->response_headers);
        }
        free(server);
    }
}
