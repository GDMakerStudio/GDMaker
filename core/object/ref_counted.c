#include "ref_counted.h"

RefCounted *ref_counted_create() {
    RefCounted *ref_counted = (RefCounted *)malloc(sizeof(RefCounted));
    if (ref_counted) {
        ref_counted->ref_count = 1; // Initial reference count is 1
    }
    return ref_counted;
}

void ref_counted_reference(RefCounted *ref_counted) {
    if (ref_counted) {
        ref_counted->ref_count++;
    }
}

void ref_counted_unreference(RefCounted *ref_counted) {
    if (ref_counted) {
        ref_counted->ref_count--;
        if (ref_counted->ref_count <= 0) {
            free(ref_counted);
        }
    }
}
