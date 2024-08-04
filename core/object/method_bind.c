#include "method_bind.h"
#include <stdlib.h>
#include <string.h>

struct MethodBind {
    MethodFunc method;
    MethodBindInfo *info;
};

MethodBind *method_bind_create(MethodFunc method) {
    MethodBind *bind = (MethodBind *)malloc(sizeof(MethodBind));
    bind->method = method;
    bind->info = NULL;
    return bind;
}

void method_bind_free(MethodBind *method_bind) {
    if (method_bind->info) {
        method_bind_info_free(method_bind->info);
    }
    free(method_bind);
}

void method_bind_call(MethodBind *method_bind, void *instance, void **args, int arg_count) {
    if (method_bind && method_bind->method) {
        method_bind->method(instance, args, arg_count);
    }
}

void method_bind_set_info(MethodBind *method_bind, const char *name, const char *args, const char *return_type) {
    if (method_bind->info) {
        method_bind_info_free(method_bind->info);
    }
    method_bind->info = method_bind_info_create(name, args, return_type);
}

MethodBindInfo *method_bind_get_info(MethodBind *method_bind) {
    return method_bind->info;
}

MethodBindInfo *method_bind_info_create(const char *name, const char *args, const char *return_type) {
    MethodBindInfo *info = (MethodBindInfo *)malloc(sizeof(MethodBindInfo));
    info->name = strdup(name);
    info->args = strdup(args);
    info->return_type = strdup(return_type);
    return info;
}

void method_bind_info_free(MethodBindInfo *info) {
    if (info) {
        free((void *)info->name);
        free((void *)info->args);
        free((void *)info->return_type);
        free(info);
    }
}

void method_bind_info_print(MethodBindInfo *info) {
    if (info) {
        printf("Method Name: %s\n", info->name);
        printf("Arguments: %s\n", info->args);
        printf("Return Type: %s\n", info->return_type);
    }
}	
