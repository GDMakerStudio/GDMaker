#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "core/string/string.h"
#include "core/variant/variant.h"
#include "core/variant/array.h"
#include "core/object/struct_db.h"
#define GDVIRTUAL_BIND(class, method) \
    void class##_##method(Object *self, ...); \
    void class##_##method(Object *self, ...) { \
        va_list args; \
        va_start(args, self); \
        for (size_t i = 0; i < self->virtual_method_count; i++) { \
            if (strcmp(self->virtual_methods[i].name, #method) == 0) { \
                self->virtual_methods[i].method(self, args); \
                break; \
            } \
        } \
        va_end(args); \
    }

// Macro to call a virtual method
#define GDVIRTUAL_CALL(obj, method, ...) do { \
    va_list args; \
    va_start(args, __VA_ARGS__); \
    for (size_t i = 0; i < (obj)->virtual_method_count; i++) { \
        if (strcmp((obj)->virtual_methods[i].name, #method) == 0) { \
            (obj)->virtual_methods[i].method((obj), args); \
            break; \
        } \
    } \
    va_end(args); \
} while(0)


#define GDARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define GDMEM_ALLOC(size) malloc(size)
#define GDMEM_REALLOC(ptr, size) realloc(ptr, size)
#define GDMEM_FREE(ptr) free(ptr)
#define GD_IS_CLASS(obj, class_name) (strcmp((obj)->class_name, class_name) == 0)
#define GDMEMNEW(p_class) ({ \
    p_class *instance = (p_class *)GDMEM_ALLOC(sizeof(p_class)); \
    if (instance) { \
        object_init((Object *)instance, #p_class); \
        if (instance->base.virtual_init) { \
            instance->base.virtual_init((Object *)instance); \
        } \
    } \
    instance; \
})


typedef struct Object Object;

typedef void (*MethodBindInfo)(Object *self, va_list args);
typedef void (*InitFunc)(Object *self);
typedef bool (*IsValidFunc)(void); // is_valid function type without parameters

// Object ID
typedef struct ObjectID {
   int id; // for object id
} ObjectID;

typedef struct {
    const char *name;
    MethodBindInfo method;
} MethodBind;

// Struct for storing property information
typedef struct {
    const char *name;
    Variant value;
} Property;

typedef struct  Method{
    const char* name;
    MethodBindInfo method;
    //


} Method;

typedef struct {
    const char *signal;
    Object *target;
    const char *method;
    int flags;
} SignalConnection;

// Object struct definition
struct Object {
    const char *class_name;
    MethodBind *methods;
    size_t method_count;
    Property *properties;
    size_t property_count;
    InitFunc virtual_init; // Pointer to the virtual init function
    SignalConnection *connections;
    size_t connection_count;
    int instance_id; // Unique instance ID
    ObjectID object_id;
    IsValidFunc is_valid;
};

void Object_register_class();

void free(Object *obj);
void add_method(Object *obj, const char *name, MethodBindInfo method);
bool has_method(Object *obj, const char *name);
void call_method(Object *obj, const char *name, ...);
void set_property(Object *obj, const char *name, Variant value);
Variant *get_property(Object *obj, const char *name);
Property *get_property_list(Object *obj, size_t *count);
void *get(Object *obj, const char *property);
void connect(Object *obj, const char *signal, Object *target, const char *method, int flags);
void disconnect(Object *obj, const char *signal, const char *method);
void emit_signal(Object *obj, const char *signal, ...);
bool is_connected(Object *obj, const char *signal, const char *method);
bool is_class(Object *obj, const char *class_name);
int get_instance_id(Object *obj);
const char *get_class(Object *obj);
void set_deferred(Object *obj, const char *property, Variant value);
const char *to_string(Object *obj);
SignalConnection *get_incoming_connections(Object *obj, size_t *count);
MethodBind *get_method_list(Object *obj, size_t *count);
bool is_valid(void);

int id_get(const ObjectID *object_id);
int id_compare(const ObjectID* id1, const ObjectID *id2);
void id_reset_counter(void);
bool id_is_valid(const ObjectID *object_id);

#define OBJECT_ID_INIT(obj_id) object_id_init(&(obj_id))
#define GET_OBJECT_ID(obj_id) object_id_get(&(obj_id))

#define GDCLASS(class_name, base_class_name) \
    typedef struct class_name { \
        base_class_name base; \
        static const char *get_class_name() { return #class_name; } \
        static const char *get_type_name() {return #class_name; } \
        static const char *get_class() { return #class_name; } \
        static String get_inshits_from_list() { return #base_class_name; } \

        friend struct StructDB; \
    };



#endif // OBJECT_H
