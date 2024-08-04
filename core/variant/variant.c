

#include "variant.h"


void variant_impl_ref(Variant *variant) {
    if (variant) {
        variant->impl->ref_count++;
    }
}

// Helper function to decrement the reference count and free if necessary
void variant_impl_unref(Variant *variant) {
    if (variant) {
        variant->impl->ref_count;
        if (variant->impl->ref_count == 0) {
            variant->destroy(variant); // Free implementation if no references remain
            free(variant);
        }
    }
}

// Function to destroy the VariantImpl
void variant_destroy_impl(Variant *variant) {
    switch (variant->impl->type) {
        case VARIANT_TYPE_STRING:
            free(variant->impl->value._string.data);
            break;
        case VARIANT_TYPE_ARRAY:
            for (size_t i = 0; i < variant->impl->value._array.length; i++) {
                variant_destroy(&variant->impl->value._array.data[i]);
            }
            free( variant->impl->value._array.data);
            break;
        case VARIANT_TYPE_DICTIONARY:
            for (size_t i = 0; i < variant->impl->value._dictionary.length; i++) {
                variant_destroy(&variant->impl->value._dictionary.keys[i]);
                variant_destroy(&variant->impl->value._dictionary.values[i]);
            }
            free(variant->impl->value._dictionary.keys);
            free(variant->impl->value._dictionary.values);
            break;
        case VARIANT_TYPE_OBJECT:
            free(variant->impl->value._object.data);
            break;
        default:
            break;
    }
}

void variant_new_nil(Variant *variant) {
    variant->impl->type = VARIANT_TYPE_NIL;
}

void variant_new_int(Variant *variant, int64_t value) {
    variant->impl->type = VARIANT_TYPE_INT;
    variant->impl->value._int = value;
}

void variant_new_float(Variant *variant, double value) {
    variant->impl->type = VARIANT_TYPE_FLOAT;
    variant->impl->value._float = value;
}

void variant_new_bool(Variant *variant, bool value) {
    variant->impl->type = VARIANT_TYPE_BOOL;
    variant->impl->value._bool = value;
}

void variant_new_string(Variant *variant, const char *value) {
    variant->impl->type = VARIANT_TYPE_STRING;
    variant->impl->value._string.length = strlen(value);
    variant->impl->value._string.data = (char *)malloc(variant->impl->value._string.length + 1);
    strcpy(variant->impl->value._string.data, value);
}

void variant_new_array(Variant *variant, Variant *data, size_t length) {
    variant->impl->type = VARIANT_TYPE_ARRAY;
    variant->impl->value._array.length = length;
    variant->impl->value._array.data = (Variant *)malloc(length * sizeof(Variant));
    for (size_t i = 0; i < length; i++) {
        variant_copy(&variant->impl->value._array.data[i], &data[i]);
    }
}

void variant_new_dictionary(Variant *variant, Variant *keys, Variant *values, size_t length) {
    variant->impl->type = VARIANT_TYPE_DICTIONARY;
    variant->impl->value._dictionary.length = length;
    variant->impl->value._dictionary.keys = (Variant *)malloc(length * sizeof(Variant));
    variant->impl->value._dictionary.values = (Variant *)malloc(length * sizeof(Variant));
    for (size_t i = 0; i < length; i++) {
        variant_copy(&variant->impl->value._dictionary.keys[i], &keys[i]);
        variant_copy(&variant->impl->value._dictionary.values[i], &values[i]);
    }
}

void variant_new_object(Variant *variant, void *data) {
    variant->impl->type = VARIANT_TYPE_OBJECT;
    variant->impl->value._object.data = data;
}

void variant_new_vector2(Variant *variant, float x, float y) {
    variant->impl->type = VARIANT_TYPE_VECTOR2;
    variant->impl->value._vector2.x = x;
    variant->impl->value._vector2.y = y;
}

void variant_new_vector3(Variant *variant, float x, float y, float z) {
    variant->impl->type = VARIANT_TYPE_VECTOR3;
    variant->impl->value._vector3.x = x;
    variant->impl->value._vector3.y = y;
    variant->impl->value._vector3.z = z;
}

void variant_destroy(Variant *variant) {
    if (variant) {
        variant_impl_unref(variant);
    }
}

void variant_copy(Variant *dest, const Variant *src) {
    variant_destroy(dest);  // Free existing data in dest
    dest->impl->type = src->impl->type;
    variant_impl_ref(dest); // Increment reference count
}

void variant_print(const Variant *variant) {
    if (!variant) {
        printf("Nil\n");
        return;
    }
    switch (variant->impl->type) {
        case VARIANT_TYPE_NIL:
            printf("Nil\n");
            break;
        case VARIANT_TYPE_INT:
            printf("Int: %ld\n", variant->impl->value._int);
            break;
        case VARIANT_TYPE_FLOAT:
            printf("Float: %f\n", variant->impl->value._float);
            break;
        case VARIANT_TYPE_BOOL:
            printf("Bool: %s\n", variant->impl->value._bool ? "true" : "false");
            break;
        case VARIANT_TYPE_STRING:
            printf("String: %s\n", variant->impl->value._string.data);
            break;
        case VARIANT_TYPE_ARRAY:
            printf("Array of length %zu:\n", variant->impl->value._array.length);
            for (size_t i = 0; i < variant->impl->value._array.length; i++) {
                printf("  [%zu] ", i);
                variant_print(&variant->impl->value._array.data[i]);
            }
            break;
        case VARIANT_TYPE_DICTIONARY:
            printf("Dictionary of length %zu:\n", variant->impl->value._dictionary.length);
            for (size_t i = 0; i < variant->impl->value._dictionary.length; i++) {
                printf("  Key [%zu]: ", i);
                variant_print(&variant->impl->value._dictionary.keys[i]);
                printf("  Value [%zu]: ", i);
                variant_print(&variant->impl->value._dictionary.values[i]);
            }
            break;
        case VARIANT_TYPE_OBJECT:
            printf("Object: %p\n", variant->impl->value._object.data);
            break;
        case VARIANT_TYPE_VECTOR2:
            printf("Vector2: (%f, %f)\n", variant->impl->value._vector2.x, variant->impl->value._vector2.y);
            break;
        case VARIANT_TYPE_VECTOR3:
            printf("Vector3: (%f, %f, %f)\n", variant->impl->value._vector3.x, variant->impl->value._vector3.y, variant->impl->value._vector3.z);
            break;
        default:
            printf("Unknown type\n");
            break;
    }
}
