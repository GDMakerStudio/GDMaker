#ifndef METHOD_BIND_POINTER_H
#define METHOD_BIND_POINTER_H

#include "method_bind.h"

typedef struct MethodBindPointer MethodBindPointer;

typedef void (*PointerMethodFunc)(void *, void **, int);

struct MethodBindPointer {
    MethodBind base; // Inherit from MethodBind
    PointerMethodFunc pointer_method;
};

MethodBindPointer *method_bind_pointer_create(PointerMethodFunc method);
void method_bind_pointer_free(MethodBindPointer *method_bind_pointer);
void method_bind_pointer_call(MethodBindPointer *method_bind_pointer, void *instance, void **args, int arg_count);

void method_bind_pointer_set_info(MethodBindPointer *method_bind_pointer, const char *name, const char *args, const char *return_type);
MethodBindInfo *method_bind_pointer_get_info(MethodBindPointer *method_bind_pointer);

#endif // METHOD_BIND_POINTER_H
