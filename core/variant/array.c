#include "array.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_push_back(Array *array, void *element) {
    if (array->size >= array->capacity) {
        array_resize(array, array->capacity * 2);
    }
    array->data[array->size++] = element;
}

void *array_emplace_back(Array *array) {
    if (array->size >= array->capacity) {
        array_resize(array, array->capacity * 2);
    }
    return array->data[array->size++];
}

void array_insert(Array *array, size_t index, void *element) {
    if (index > array->size) {
        return;
    }
    if (array->size >= array->capacity) {
        array_resize(array, array->capacity * 2);
    }
    for (size_t i = array->size; i > index; i--) {
        array->data[i] = array->data[i - 1];
    }
    array->data[index] = element;
    array->size++;
}

void array_erase(Array *array, size_t index) {
    if (index >= array->size) {
        return;
    }
    for (size_t i = index; i < array->size - 1; i++) {
        array->data[i] = array->data[i + 1];
    }
    array->size--;
}

void array_replace(Array *array, size_t index, void *element) {
    if (index < array->size) {
        array->data[index] = element;
    }
}

void *array_get(Array *array, size_t index) {
    if (index >= array->size) {
        return NULL;
    }
    return array->data[index];
}

void array_clear(Array *array, int free_data) {
    if (free_data) {
        for (size_t i = 0; i < array->size; i++) {
            free(array->data[i]);
        }
    }
    array->size = 0;
}

int array_is_empty(Array *array) {
    return array->size == 0;
}

size_t array_size(const Array *array) {
    return array->size;
}

size_t array_capacity(const Array *array) {
    return array->capacity;
}

void array_resize(Array *array, size_t new_capacity) {
    if (new_capacity < array->size) {
        array->size = new_capacity;
    }
    array->data = (void **)realloc(array->data, new_capacity * sizeof(void *));
    if (!array->data) {
        perror("Failed to reallocate memory for array data");
        exit(EXIT_FAILURE);
    }
    array->capacity = new_capacity;
}

void array_free(Array *array, int free_data) {
    array_clear(array, free_data);
    free(array->data);
}

Iterator array_iterator(Array *array) {
    Iterator it = {array, 0};
    return it;
}

void *iterator_get(Iterator *it) {
    if (it->index < it->array->size) {
        return it->array->data[it->index];
    }
    return NULL;
}

void iterator_next(Iterator *it) {
    if (it->index < it->array->size) {
        it->index++;
    }
}

int iterator_end(Iterator *it) {
    return it->index >= it->array->size;
}

ConstIterator array_const_iterator(const Array *array) {
    ConstIterator it = {array, 0};
    return it;
}

const void *const_iterator_get(const ConstIterator *it) {
    if (it->index < it->array->size) {
        return it->array->data[it->index];
    }
    return NULL;
}

void const_iterator_next(ConstIterator *it) {
    if (it->index < it->array->size) {
        it->index++;
    }
}

int const_iterator_end(const ConstIterator *it) {
    return it->index >= it->array->size;
}

size_t array_find(Array *array, void *element) {
    for (size_t i = 0; i < array->size; i++) {
        if (array->data[i] == element) {
            return i;
        }
    }
    return (size_t)-1;
}

void array_reverse(Array *array) {
    for (size_t i = 0; i < array->size / 2; i++) {
        void *temp = array->data[i];
        array->data[i] = array->data[array->size - 1 - i];
        array->data[array->size - 1 - i] = temp;
    }
}

void array_sort(Array *array, int (*compare)(const void *, const void *)) {
    qsort(array->data, array->size, sizeof(void *), compare);
}

Array array_copy(const Array *array) {
    Array new_array = {NULL, array->size, array->capacity};
    new_array.data = (void **)malloc(new_array.capacity * sizeof(void *));
    if (!new_array.data) {
        perror("Failed to allocate memory for array copy");
        exit(EXIT_FAILURE);
    }
    memcpy(new_array.data, array->data, array->size * sizeof(void *));
    return new_array;
}

int array_contains(Array *array, void *element) {
    return array_find(array, element) != (size_t)-1;
}

size_t array_count(Array *array, void *element) {
    size_t count = 0;
    for (size_t i = 0; i < array->size; i++) {
        if (array->data[i] == element) {
            count++;
        }
    }
    return count;
}

void array_extend(Array *dest, const Array *src) {
    for (size_t i = 0; i < src->size; i++) {
        array_push_back(dest, src->data[i]);
    }
}

void array_map(Array *array, void (*func)(void *)) {
    for (size_t i = 0; i < array->size; i++) {
        func(array->data[i]);
    }
}

Array array_filter(const Array *array, int (*predicate)(const void *)) {
    Array new_array = {NULL, 0, ARRAY_INIT_CAPACITY};
    new_array.data = (void **)malloc(new_array.capacity * sizeof(void *));
    if (!new_array.data) {
        perror("Failed to allocate memory for filtered array");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < array->size; i++) {
        if (predicate(array->data[i])) {
            if (new_array.size >= new_array.capacity) {
                array_resize(&new_array, new_array.capacity * 2);
            }
            new_array.data[new_array.size++] = array->data[i];
        }
   }
    return new_array;
}
