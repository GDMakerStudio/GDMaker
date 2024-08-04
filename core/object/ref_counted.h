#ifndef REF_COUNTED_H
#define REF_COUNTED_H

#include <stdlib.h>

typedef struct RefCounted RefCounted;

struct RefCounted {
    int ref_count;
};

// Create a new RefCounted object
RefCounted *ref_counted_create();

// Increment the reference count
void ref_counted_reference(RefCounted *ref_counted);

// Decrement the reference count and free the object if the count reaches zero
void ref_counted_unreference(RefCounted *ref_counted);

#endif // REF_COUNTED_H
