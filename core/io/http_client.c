#include "http_client.h"

#include <stdio.h>
#include <string.h>


#include "bake/bake_config.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    HttpClient* client = (HttpClient*)mem->memory;

    char *ptr = realloc(client->body_size ? client->body_size : mem->memory, client->downloaded_bytes + realsize + 1);
    if(ptr == NULL) {
        // out of memory!
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    client->body_size = ptr;
    memcpy(&(client->body_size[client->downloaded_bytes]), contents, realsize);
    client->downloaded_bytes += realsize;
    client->body_size[client->downloaded_bytes] = 0;

    return realsize;
}

char* http_client_request(HttpClient* self, const char* url, const char** custom_headers, const char* method, const char* request_data) {
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = (char*)self;  // pass self as the first element of the struct
    chunk.size = 0;              // no data at this point

    self->curl_handle = curl_easy_init();

    // specify URL
    curl_easy_setopt(self->curl_handle, CURLOPT_URL, url);

    // set HTTP method
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

    // set proxy if specified
    if (self->proxy_host) {
        char proxy[256];
        snprintf(proxy, sizeof(proxy), "%s:%d", self->proxy_host, self->proxy_port);
        curl_easy_setopt(self->curl_handle, CURLOPT_PROXY, proxy);
    }

    // send all data to this function
    curl_easy_setopt(self->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    // we pass our 'chunk' struct to the callback function
    curl_easy_setopt(self->curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    // some servers don't like requests that are made without a user-agent field, so we provide one
    curl_easy_setopt(self->curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // add custom headers if present
    if (custom_headers) {
        struct curl_slist *headers = NULL;
        for (size_t i = 0; custom_headers[i]; ++i) {
            headers = curl_slist_append(headers, custom_headers[i]);
        }
        curl_easy_setopt(self->curl_handle, CURLOPT_HTTPHEADER, headers);
        self->headers = headers;  // store for cleanup
    }

    // perform the request
    res = curl_easy_perform(self->curl_handle);

    // check for errors
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(self->body_size);
        self->body_size = NULL;
    }

    // cleanup curl stuff
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
