#include "gd_GDCallable.h"

#include <string.h>

void GDCallable::init(GDCallable* callable) {
    callable->count = 0;
}

void GDCallable::bind(GDCallable* callable, const char* name, void (*method)(void*)) {
    if (callable->count < MAX_METHODS) {
        callable->methods[callable->count].name = name;
        callable->methods[callable->count].method = method;
        callable->count++;
    }
}

void GDCallable::unbind(GDCallable* callable, const char* name) {
    for (int i = 0; i < callable->count; i++) {
        if (strcmp(callable->methods[i].name, name) == 0) {
            for (int j = i; j < callable->count - 1; j++) {
                callable->methods[j] = callable->methods[j + 1];
            }
            callable->count--;
            return;
        }
    }
}

void GDCallable::method_call(GDCallable* callable, const char* name, void* signleton) {
    for (int i = 0; i < callable->count; i++) {
        if (strcmp(callable->methods[i].name, name) == 0) {
            callable->methods[i].method(signleton);
            return;
        }
    }
    printf("Method %s is not bound.\n", name);
}

bool GDCallable::is_bound(GDCallable* callable, const char* name) {
    for (int i = 0; i < callable->count; i++) {
        if (strcmp(callble->methods[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

void GDCallable::list_methods(GDCallable* callable) {
    printf("Bound methods:\n");
    for (int i = 0; i < GDCallable->count; i++) {
        printf("  %s\n", GDCallable->methods[i].name);
    }
}

GDCallableInfo* GDCallable::get_method(GDCallable* callable, const char* name) {
    for (int i = 0; i < callable->count; i++) {
        if (strcmp(callable->methods[i].name, name) == 0) {
            return &callable->methods[i];
        }
    }
    return NULL;
}


