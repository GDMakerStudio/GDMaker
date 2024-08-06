#ifndef STRUCT_DB_H
#define STRUCT_DB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Method {
    const char *name;
    void (*func)(void *instance);
} Method;

typedef struct Property {
    const char *name;
} Property;

typedef struct EnumConstant {
    const char *name;
    int value;
} EnumConstant;

typedef struct Enum {
    const char *name;
    EnumConstant *constants;
    int constant_count;
} Enum;

typedef struct Signal {
    const char *name;
} Signal;

typedef void* (*ConstructorFunc)();
typedef void (*DestructorFunc)(void*);

typedef struct StructInfo {
    const char *name;
    const char *parent_class_name;
    Method *methods;
    int method_count;
    Property *properties;
    int property_count;
    Enum *enums;
    int enum_count;
    Signal *signals;
    int signal_count;
    struct StructInfo **inheriters;
    int inheriter_count;
    ConstructorFunc constructor;
    DestructorFunc destructor;
} StructInfo;

typedef struct StructDB {
    StructInfo *structs;
    int struct_count;
} StructDB;





void add_enum(StructDB *db, const char *class_name, const char *enum_name, EnumConstant *constants, int constant_count);
Enum* get_class_enum_list(StructDB *db, const char *class_name, int *count, int no_inheritance);
EnumConstant* get_class_enum_constants(StructDB *db, const char *class_name, const char *enum_name, int *count, int no_inheritance);
int get_class_integer_constant_enum(StructDB *db, const char *class_name, const char *name, int no_inheritance);
int get_class_integer_constant(StructDB *db, const char *class_name, const char *name);
Property* get_class_property(StructDB *db, void *object, const char *property_name);
Method* get_class_method_list(StructDB *db, const char *class_name, int *count, int no_inheritance);
Signal* get_class_signal_list(StructDB *db, const char *class_name, int *count, int no_inheritance);
Signal* get_class_signal(StructDB *db, const char *class_name, const char *signal_name);
int has_method(StructDB *db, const char *class_name, const char *method_name, int no_inheritance);
int has_integer_constant(StructDB *db, const char *class_name, const char *name);
StructInfo* find_struct_info(StructDB *db, const char *class_name);
int is_parent_class(StructDB *db, const char *class_name, const char *inherits);
StructInfo* get_inheriters_from_class(StructDB *db, const char *class_name, int *count);
const char* get_parent_class(StructDB *db, const char *class_name);
void* instantiate(StructDB *db, const char *class_name);
void destroy_instance(StructDB *db, const char *class_name, void *instance);
void add_method(StructDB *db, const char *class_name, const char *method_name, void (*func)(void *instance));
void add_property(StructDB *db, const char *class_name, const char *property_name);
void bind_method(StructDB *db, const char *class_name, const char *method_name, void (*func)(void *instance));
void register_class(StructDB *db , const char *p_class);
void unregister_class(StructDB *db, const char *class_name);

#define GDREGISTER_CLASS(db, p_class) register_class(db, #p_class);
#define BIND_METHOD(db, class_name, method_name, method_func) bind_method(db, class_name, method_name, method_func);


#endif // STRUCT_DB_H
