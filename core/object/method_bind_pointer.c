#include "method_bind_pointer.h"

#include <stdlib.h>
#include <string.h>

MethodBindPointer *method_bind_pointer_create(PointerMethodFunc method) {
    MethodBindPointer *bind_pointer = (MethodBindPointer *)malloc(sizeof(MethodBindPointer));
    bind_pointer->base.method = (MethodFunc)method;
    bind_pointer->pointer_method = method;
    bind_pointer->base.info = NULL;
    return bind_pointer;
}

void method_bind_pointer_free(MethodBindPointer *method_bind_pointer) {
    if (method_bind_pointer) {
        if (method_bind_pointer->base.info) {
            method_bind_info_free(method_bind_pointer->base.info);
        }
        free(method_bind_pointer);
    }
}

void method_bind_pointer_call(MethodBindPointer *method_bind_pointer, void *instance, void **args, int arg_count) {
    if (method_bind_pointer && method_bind_pointer->pointer_method) {
        method_bind_pointer->pointer_method(instance, args, arg_count);
    }
}

void method_bind_pointer_set_info(MethodBindPointer *method_bind_pointer, const char *name, const char *args, const char *return_type) {
    if (method_bind_pointer->base.info) {
        method_bind_info_free(method_bind_pointer->base.info);
    }
    method_bind_pointer->base.info = method_bind_info_create(name, args, return_type);
}

MethodBindInfo *method_bind_pointer_get_info(MethodBindPointer *method_bind_pointer) {
    return method_bind_pointer->base.info;
}

