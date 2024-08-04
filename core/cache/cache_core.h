#ifndef CACHE_CORE_H
#define CACHE_CORE_H

#include <stddef.h>

// Define the CacheCore structure
typedef struct {
    void* data;
    size_t size;
    size_t capacity; // Added capacity to track the maximum size
} CacheCore;

// Function prototypes
CacheCore* cache_core_create(size_t capacity);
void cache_core_destroy(CacheCore* cache);
void cache_core_set_data(CacheCore* cache, void* data, size_t size);
void* cache_core_get_data(CacheCore* cache);
size_t cache_core_get_size(CacheCore* cache);
size_t cache_core_get_capacity(CacheCore* cache); // Get the capacity of the cache
void cache_core_clear(CacheCore* cache); // Clear the cache
int cache_core_resize(CacheCore* cache, size_t new_capacity); // Resize the cache
int cache_core_is_empty(CacheCore* cache); // Check if the cache is empty
void cache_core_copy_data(CacheCore* dest, CacheCore* src); // Copy data from one cache to another

#endif // CACHE_CORE_H
