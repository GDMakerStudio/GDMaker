#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    void **data;      // Pointer to the array of elements
    size_t size;      // Current number of elements
    size_t capacity;  // Maximum number of elements the array can hold
} Array;

typedef struct {
    Array *array;    // Pointer to the Array
    size_t index;    // Current index
} Iterator;

typedef struct {
    const Array *array; // Pointer to the Array
    size_t index;       // Current index
} ConstIterator;

#define ARRAY_INIT_CAPACITY 10
#define ARRAY_COUNT(arr) ((arr).size)
#define ARRAY_CAPACITY(arr) ((arr).capacity)
#define ARRAY_IS_EMPTY(arr) ((arr).size == 0)
#define ARRAY_GET(arr, idx) ((arr).data[idx])
#define ARRAY_FRONT(arr) ((arr).data[0])
#define ARRAY_BACK(arr) ((arr).data[(arr).size - 1])
#define ARRAY_END(arr) ((arr).data[(arr).size])

// Function prototypes
void array_push_back(Array *array, void *element);
void *array_emplace_back(Array *array);
void array_insert(Array *array, size_t index, void *element);
void array_erase(Array *array, size_t index);
void array_replace(Array *array, size_t index, void *element);
void *array_get(Array *array, size_t index);
void array_clear(Array *array, int free_data);
int array_is_empty(Array *array);
size_t array_size(const Array *array);
size_t array_capacity(const Array *array);
void array_resize(Array *array, size_t new_capacity);
void array_free(Array *array, int free_data);
Iterator array_iterator(Array *array);
void *iterator_get(Iterator *it);
void iterator_next(Iterator *it);
int iterator_end(Iterator *it);
ConstIterator array_const_iterator(const Array *array);
const void *const_iterator_get(const ConstIterator *it);
void const_iterator_next(ConstIterator *it);
int const_iterator_end(const ConstIterator *it);
size_t array_find(Array *array, void *element);
void array_reverse(Array *array);
void array_sort(Array *array, int (*compare)(const void *, const void *));
Array array_copy(const Array *array);
int array_contains(Array *array, void *element);
size_t array_count(Array *array, void *element);
void array_extend(Array *dest, const Array *src);
void array_map(Array *array, void (*func)(void *));
Array array_filter(const Array *array, int (*predicate)(const void *));

#endif // ARRAY_H
