#include "http_client.h"

#include <stdio.h>
#include <string.h>


#include "bake/bake_config.h"
#include "core/error/error_macros.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};



char* http_client_request(HttpClient* self, const char* url, const char** custom_headers, const char* method, const char* request_data) {
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = (char*)self;  // pass self as the first element of the struct
    chunk.size = 0;
    self->curl_handle = curl_easy_init();
    curl_easy_setopt(self->curl_handle, CURLOPT_URL, url);

    if (method && strcmp(method, "POST") == 0) {
        curl_easy_setopt(self->curl_handle, CURLOPT_POST, 1L);
        if (request_data) {
            curl_easy_setopt(self->curl_handle, CURLOPT_POSTFIELDS, request_data);
        }
    } else if (method && strcmp(method, "PUT") == 0) {
        curl_easy_setopt(self->curl_handle, CURLOPT_CUSTOMREQUEST, "PUT");
        if (request_data) {
            curl_easy_setopt(self->curl_handle, CURLOPT_POSTFIELDS, request_data);
        }
    } else if (method && strcmp(method, "DELETE") == 0) {
        curl_easy_setopt(self->curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
    } else {
        curl_easy_setopt(self->curl_handle, CURLOPT_HTTPGET, 1L);
    }
    if (self->proxy_host) {
        char proxy[256];
        snprintf(proxy, sizeof(proxy), "%s:%d", self->proxy_host, self->proxy_port);
        curl_easy_setopt(self->curl_handle, CURLOPT_PROXY, proxy);
    }
    curl_easy_setopt(self->curl_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(self->curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(self->curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    if (custom_headers) {
        struct curl_slist *headers = NULL;
        for (size_t i = 0; custom_headers[i]; ++i) {
            headers = curl_slist_append(headers, custom_headers[i]);
        }
        curl_easy_setopt(self->curl_handle, CURLOPT_HTTPHEADER, headers);
        self->headers = headers; 
    }
    res = curl_easy_perform(self->curl_handle);
    if(res != CURLE_OK) {
        ERR_FAIL_COND_MSG("the func can't be call curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(self->body_size);
        self->body_size = NULL;
    }
    curl_easy_cleanup(self->curl_handle);
    return self->body_size;
}

void http_client_set_http_proxy(HttpClient* self, const char* host, int port) {
    if (self->proxy_host) {
        free(self->proxy_host);
    }
    self->proxy_host = strdup(host);
    self->proxy_port = port;
}

size_t http_client_get_downloaded_bytes(HttpClient* self) {
    return self->downloaded_bytes;
}

size_t http_client_get_body_size(HttpClient* self) {
    return self->body_size ? strlen(self->body_size) : 0;
}

void http_client_cancel_request(HttpClient* self) {
    if (self->curl_handle) {
        curl_easy_cleanup(self->curl_handle);
        self->curl_handle = NULL;
    }
}

HttpClient* http_client_new() {
    HttpClient* client = (HttpClient*)malloc(sizeof(HttpClient));
    client->request = http_client_request;
    client->get_downloaded_bytes = http_client_get_downloaded_bytes;
    client->get_body_size = http_client_get_body_size;
    client->cancel_request = http_client_cancel_request;
    client->set_http_proxy = http_client_set_http_proxy;
    client->headers = NULL;
    client->downloaded_bytes = 0;
    client->body_size = NULL;
    client->curl_handle = NULL;
    client->proxy_host = NULL;
    client->proxy_port = 0;
    return client;
}

void http_client_free(HttpClient* client) {
    if (client) {
        if (client->headers) {
            curl_slist_free_all(client->headers);
        }
        if (client->body_size) {
            free(client->body_size);
        }
        if (client->proxy_host) {
            free(client->proxy_host);
        }
        free(client);
    }
}
