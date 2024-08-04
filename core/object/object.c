

#include "object.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



#include "core/variant/variant.h"
#include "core/string/string.h"



void add_method(Object *obj, const char *name, MethodBindInfo method) {
    obj->methods = GDMEM_REALLOC(obj->methods, sizeof(MethodBind) * (obj->method_count + 1));
    obj->methods[obj->method_count].name = name;
    obj->methods[obj->method_count].method = method;
    obj->method_count++;
}

bool has_method(Object *obj, const char *name) {
    for (size_t i = 0; i < obj->method_count; i++) {
        if (strcmp(obj->methods[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

void call_method(Object *obj, const char *name, ...) {
    for (size_t i = 0; i < obj->method_count; i++) {
        if (strcmp(obj->methods[i].name, name) == 0) {
            va_list args;
            va_start(args, name);
            obj->methods[i].method(obj, args);
            va_end(args);
            return;
        }
    }
}

void set_property(Object *obj, const char *name, Variant value) {
    for (size_t i = 0; i < obj->property_count; i++) {
        if (strcmp(obj->properties[i].name, name) == 0) {
            obj->properties[i].value = value;
            return;
        }
    }
    obj->properties = GDMEM_REALLOC(obj->properties, sizeof(Property) * (obj->property_count + 1));
    obj->properties[obj->property_count].name = name;
    obj->properties[obj->property_count].value = value;
    obj->property_count++;
}

Variant *get_property(Object *obj, const char *name) {
    for (size_t i = 0; i < obj->property_count; i++) {
        if (strcmp(obj->properties[i].name, name) == 0) {
            return &obj->properties[i].value;
        }
    }
    return NULL;
}

Property *get_property_list(Object *obj, size_t *count) {
    *count = obj->property_count;
    return obj->properties;
}

void *get(Object *obj, const char *property) {
    Variant *value = get_property(obj, property);
    if (value) {
        return VARIANT_TYPE_STRING;
    }
    return NULL;
}

void connect(Object *obj, const char *signal, Object *target, const char *method, int flags) {
    obj->connections = GDMEM_REALLOC(obj->connections, sizeof(SignalConnection) * (obj->connection_count + 1));
    obj->connections[obj->connection_count].signal = signal;
    obj->connections[obj->connection_count].target = target;
    obj->connections[obj->connection_count].method = method;
    obj->connections[obj->connection_count].flags = flags;
    obj->connection_count++;
}

bool is_connected(Object *obj, const char *signal, const char *method) {
    for (size_t i = 0; i < obj->connection_count; i++) {
        if (strcmp(obj->connections[i].signal, signal) == 0 && strcmp(obj->connections[i].method, method) == 0) {
            return true;
        }
    }
    return false;
}

bool is_class(Object *obj, const char *class_name) {
    return strcmp(obj->class_name, class_name) == 0;
}

int get_instance_id(Object *obj) {
    return obj->instance_id;
}


void disconnect(Object *obj, const char *signal, const char *method) {
    for (size_t i = 0; i < obj->connection_count; i++) {
        if (strcmp(obj->connections[i].signal, signal) == 0 && strcmp(obj->connections[i].method, method) == 0) {
            // remove the connection by shifting the remaining connections
            for (size_t j = i; j < obj->connection_count - 1; j++) {
                obj->connections[j] = obj->connections[j + 1];
            }
            obj->connection_count--;
            obj->connections = GDMEM_REALLOC(obj->connections, sizeof(SignalConnection) * obj->connection_count);
            return;
        }
    }
}

void emit_signal(Object *obj, const char *signal, ...) {
    for (size_t i = 0; i < obj->connection_count; i++) {
        if (strcmp(obj->connections[i].signal, signal) == 0) {
            Object *target = obj->connections[i].target;
            const char *method = obj->connections[i].method;
            va_list args;
            va_start(args, signal);
            call_method(target, method, args);
            va_end(args);
        }
    }
}

const char *to_string(Object *obj) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "[Object: %s, Instance ID: %d]", obj->class_name, obj->instance_id);
    return buffer;
}

SignalConnection *get_incoming_connections(Object *obj, size_t *count) {
    *count = obj->connection_count;
    return obj->connections;
}

MethodBind *get_method_list(Object *obj, size_t *count) {
    *count = obj->method_count;
    return obj->methods;
}

#define this

bool is_valid(void) {
    const Object *obj = (const Object *)((char *)this - offsetof(Object, is_valid)); // Retrieve the Object instance from the function pointer
    return id_is_valid(&obj->object_id); // Check ObjectID validity using macro
}
