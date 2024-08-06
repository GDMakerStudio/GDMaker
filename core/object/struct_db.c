#include "struct_db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/variant/variant.h"
#include "core/error/error_macros.h"
#include "core/error/error_list.h"


void register_class(StructDB *db, const char *class_name) {
    //  you should need for check if the class already exists
    if (find_struct_info(db, class_name)) {
        fprintf(stderr, "Class '%s' is already registered.\n", class_name);
        return;
    }

    StructInfo *new_class = malloc(sizeof(StructInfo));
    if (!new_class) {
        fprintf(stderr, "Memory allocation failed for class '%s'!\n", class_name);
        return;
    }

    new_class->name = strdup(class_name);
    if (!new_class->name) {
        fprintf(stderr, "Memory allocation failed for class name '%s'!\n", class_name);
        free(new_class);
        return;
    }
    new_class->parent_class_name = NULL;
    new_class->methods = NULL; 
    new_class->method_count = 0; 
    new_class->properties = NULL;
    new_class->property_count = 0;
    new_class->enums = NULL; 
    new_class->enum_count = 0; 
    new_class->signals = NULL; 
    new_class->signal_count = 0;
    new_class->inheriters = NULL; 
    new_class->inheriter_count = 0; 

    // resize the database to accommodate the new class
    StructInfo **new_structs = realloc(db->structs, (db->struct_count + 1) * sizeof(StructInfo *));
    if (!new_structs) {
        fprintf(stderr, "failed memory reallocation failed for struct database!\n");
        free(new_class->name);
        free(new_class);
        return;
    }
    db->structs = new_structs; // update the database struct pointer
    db->structs[db->struct_count] = new_class;
    db->struct_count++; 
}


StructInfo* find_struct_info(StructDB *db, const char *class_name) {
    for (int i = 0; i < db->struct_count; i++) {
        if (strcmp(db->structs[i].name, class_name) == 0) {
            return &db->structs[i];
        }
    }
    return NULL;
}


int is_parent_class(StructDB *db, const char *class_name, const char *inherits) {
    StructInfo *info = find_struct_info(db, class_name);
    while (info) {
        if (strcmp(info->name, inherits) == 0) {
            return 1;
        }
        info = find_struct_info(db, info->parent_class_name);
    }
    return 0;
}


StructInfo* get_inheriters_from_class(StructDB *db, const char *class_name, int *count) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        *count = info->inheriter_count;
        return info->inheriters;
    }
    *count = 0;
    return NULL;
}

int get_class_integer_constant_enum(StructDB *db, const char *class_name, const char *name, int no_inheritance) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        for (int i = 0; i < info->enum_count; i++) {
            for (int j = 0; j < info->enums[i].constant_count; j++) {
                if (strcmp(info->enums[i].constants[j].name, name) == 0) {
                    return info->enums[i].constants[j].value;
                }
            }
        }
    }
    return 0;
}


const char* get_parent_class(StructDB *db, const char *class_name) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        return info->parent_class_name;
    }
    return NULL;
}


void* instantiate(StructDB *db, const char *class_name) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info && info->constructor) {
        return info->constructor();
    }
    return NULL;
}


void destroy_instance(StructDB *db, const char *class_name, void *instance) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info && info->destructor) {
        info->destructor(instance);
    }
    free(instance);
}



Property* get_class_property(StructDB *db, void *object, const char *property_name) {
    StructInfo *info = find_struct_info(db, property_name);
    if (info) {
        for (int i = 0; i < info->property_count; i++) {
            if (strcmp(info->properties[i].name, property_name) == 0) {
                return &info->properties[i];
            }

        }
    }
    return NULL;
}


Method* get_class_method_list(StructDB *db, const char *class_name, int *count, int no_inheritance) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        *count = info->method_count;
        return info->methods;
    }
    *count = 0;
    return NULL;
}


int get_class_integer_constant_enum(StructDB *db, const char *class_name, const char *name, int no_inheritance) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        for (int i = 0; i < info->enum_count; i++) {
            for (int j = 0; j < info->enums[i].constant_count; j++) {
                if (strcmp(info->enums[i].constants[j].name, name) == 0) {
                    return info->enums[i].constants[j].value;
                }
            }
        }
    }
    return 0;
}

void add_enum(StructDB *db, const char *class_name, const char *enum_name, EnumConstant *constants, int constant_count) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        info->enums = realloc(info->enums, (info->enum_count + 1) * sizeof(Enum));
        Enum *new_enum = &info->enums[info->enum_count];
        new_enum->name = enum_name;
        new_enum->constants = constants;
        new_enum->constant_count = constant_count;
        info->enum_count++;
    }
}


Signal* get_class_signal(StructDB *db, const char *class_name, const char *signal_name) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        for (int i = 0; i < info->signal_count; i++) {
            if (strcmp(info->signals[i].name, signal_name) == 0) {
                return &info->signals[i];
            }
        }
    }
    return NULL;
}

int has_method(StructDB *db, const char *class_name, const char *method_name, int no_inheritance) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        for (int i = 0; i < info->method_count; i++) {
            if (strcmp(info->methods[i].name, method_name) == 0) {
                return 1;
            }
            if (no_inheritance) {
                break;
            }
        }
        info = find_struct_info(db, info->parent_class_name);
    }
    return 0;
}

int has_integer_constant(StructDB *db, const char *class_name, const char *name) {
    return get_class_integer_constant_enum(db, class_name, name, 0) != 0;
}



void bind_method(StructDB *db, const char *class_name, const char *method_name, void (*method_func)(void *)) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        info->methods = realloc(info->methods, (info->method_count + 1) * sizeof(Method));
        Method *new_method = &info->methods[info->method_count];
        new_method->name = strdup(method_name);
        new_method->func = method_func;
        info->method_count++;
    }
}

void add_method(StructDB *db, const char *class_name, const char *method_name, void (*func)(void *instance)) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        info->methods = realloc(info->methods, (info->method_count + 1) * sizeof(Method));
        info->methods[info->method_count].name = method_name;
        info->methods[info->method_count].func = func;
        info->method_count++;
    }
}

void add_property(StructDB *db, const char *class_name, const char *property_name) {
    StructInfo *info = find_struct_info(db, class_name);
    if (info) {
        info->properties = realloc(info->properties, (info->property_count + 1) * sizeof(Property));
        info->properties[info->property_count].name = property_name;
        info->property_count++;
    }
}

