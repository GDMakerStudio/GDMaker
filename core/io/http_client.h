#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stdlib.h>
#include "bake/bake_config.h"

typedef struct HttpClient {
    char* (*request)(struct HttpClient* self, const char* url, const char** custom_headers, const char* method, const char* request_data);
    size_t (*get_downloaded_bytes)(struct HttpClient* self);
    size_t (*get_body_size)(struct HttpClient* self);
    void (*cancel_request)(struct HttpClient* self);
    void (*set_http_proxy)(struct HttpClient* self, const char* host, int port);
    struct curl_slist *headers; // Custom headers
    size_t downloaded_bytes;
    size_t body_size;
    CURL* curl_handle;
    char* proxy_host;
    int proxy_port;
} HttpClient;

char* http_client_request(HttpClient* self, const char* url, const char** custom_headers, const char* method, const char* request_data);
size_t http_client_get_downloaded_bytes(HttpClient* self);
size_t http_client_get_body_size(HttpClient* self);
void http_client_cancel_request(HttpClient* self);
void http_client_set_http_proxy(HttpClient* self, const char* host, int port);
HttpClient* http_client_new();
void http_client_free(HttpClient* client);

#endif // HTTP_CLIENT_H
