#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct HttpServer {
    bool (*start)(struct HttpServer* self, int port);
    void (*handle_request)(struct HttpServer* self);
    void (*stop)(struct HttpServer* self);
    size_t (*get_response_body_length)(struct HttpServer* self);
    bool (*connect_to_host)(struct HttpServer* self, const char* host, int port);
    void (*close)(struct HttpServer* self);
    int (*get_response_code)(struct HttpServer* self);
    char* (*get_response_headers)(struct HttpServer* self);
    const char* (*get_status)(struct HttpServer* self);
    char* (*query_string_from_dict)(struct HttpServer* self, const char** dict);
    void (*poll)(struct HttpServer* self);
    int server_fd;
    bool running;
    size_t response_body_length;
    int response_code;
    char* response_headers;
} HttpServer;

bool http_server_start(HttpServer* self, int port);
void http_server_handle_request(HttpServer* self);
void http_server_stop(HttpServer* self);
size_t http_server_get_response_body_length(HttpServer* self);
bool http_server_connect_to_host(HttpServer* self, const char* host, int port);
void http_server_close(HttpServer* self);
int http_server_get_response_code(HttpServer* self);
char* http_server_get_response_headers(HttpServer* self);
const char* http_server_get_status(HttpServer* self);
char* http_server_query_string_from_dict(HttpServer* self, const char** dict);
void http_server_poll(HttpServer* self);
HttpServer* http_server_new();
void http_server_free(HttpServer* server);

#endif // HTTP_SERVER_H
