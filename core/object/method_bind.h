#ifndef METHOD_BIND_H
#define METHOD_BIND_H

#include <stdio.h>

typedef struct MethodBind MethodBind;
typedef struct MethodBindInfo MethodBindInfo;

typedef void (*MethodFunc)(void *, void **, int);

struct MethodBind {
    MethodFunc method;
};

struct MethodBindInfo {
    const char *name;
    const char *args;
    const char *return_type;
};

MethodBind *method_bind_create(MethodFunc method);
void method_bind_free(MethodBind *method_bind);
void method_bind_call(MethodBind *method_bind, void *instance, void **args, int arg_count);

void method_bind_set_info(MethodBind *method_bind, const char *name, const char *args, const char *return_type);
MethodBindInfo *method_bind_get_info(MethodBind *method_bind);

MethodBindInfo *method_bind_info_create(const char *name, const char *args, const char *return_type);
void method_bind_info_free(MethodBindInfo *info);
void method_bind_info_print(MethodBindInfo *info);

#endif // METHOD_BIND_H
