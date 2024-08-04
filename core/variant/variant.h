#ifndef VARIANT_H
#define VARIANT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define VARIANT_ENUM_CAST(p_enum) (Variant){ .type = VARIANT_TYPE_ENUM, .enum_value = (p_enum) }




typedef enum {
    VARIANT_TYPE_NIL,
    VARIANT_TYPE_INT,
    VARIANT_TYPE_FLOAT,
    VARIANT_TYPE_BOOL,
    VARIANT_TYPE_STRING,
    VARIANT_TYPE_ARRAY,
    VARIANT_TYPE_DICTIONARY,
    VARIANT_TYPE_OBJECT,
    VARIANT_TYPE_VECTOR2,
    VARIANT_TYPE_VECTOR3,
    VARIANT_TYPE_ENUM
} VariantType;

typedef struct {
    char *data;
    size_t length;
} String;

typedef struct {
    struct Variant *data;
    size_t length;
} Array;

typedef struct {
    struct Variant *keys;
    struct Variant *values;
    size_t length;
} Dictionary;

typedef struct {
    void *data;
} Object;

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct VariantImpl {
    VariantType type;
    union {
        int64_t _int;
        double _float;
        bool _bool;
        String _string;
        Array _array;
        Dictionary _dictionary;
        Object _object;
        Vector2 _vector2;
        Vector3 _vector3;
	int enum_value;
    } value;
    size_t ref_count; // Reference count for memory management
};

typedef struct Variant {
    struct VariantImpl *impl;
    struct VariantImpl type;
    // Function pointers for methods
    void (*new_nil)(struct Variant *);
    void (*new_int)(struct Variant *, int64_t value);
    void (*new_float)(struct Variant *, double value);
    void (*new_bool)(struct Variant *, bool value);
    void (*new_string)(struct Variant *, const char *value);
    void (*new_array)(struct Variant *, struct Variant *data, size_t length);
    void (*new_dictionary)(struct Variant *, struct Variant *keys, struct Variant *values, size_t length);
    void (*new_object)(struct Variant *, void *data);
    void (*new_vector2)(struct Variant *, float x, float y);
    void (*new_vector3)(struct Variant *, float x, float y, float z);
    void (*destroy)(struct Variant *);
    void (*copy)(struct Variant *dest, const struct Variant *src);
    void (*print)(const struct Variant *);
} Variant;



#endif // VARIANT_H
