#ifndef CLASS_H
#define CLASS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define GDCLASS(p_class) \
typedef struct p_class p_class; \
struct p_class


GDCLASS(ClassInfo) {
    const char* name;
    const char* p_name;
    void (*create_func)();
    size_t propert_count;
    const char **property;
    bool is_enabled; // New field to indicate if the class is enabled

};

GDCLASS(Class) {
    ClassInfo p_info;
    static Class& (get_signleton)(void);
    const char (get_class)(void);
    const char (get_class_list)(void);
    int (find_class)(const char class_name);
    bool (class_exists)(const char p_class);
    bool (can_instantiate)(const char name);
    const char (class_get_property_list)(const char *class_name, bool no_inheritance, size_t *property_count);
    const char (class_get_property)(void *object, const char *property);
    bool register_class(const char *class_name, const char *base_class_name, void *(*create_func)());
    void* (instantiate)(const char p_class);
    bool (is_class_enabled)(const char* p_class);

};




#endif // CLASS_H
