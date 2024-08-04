#include "cache_core.h"

#include <stdlib.h>
#include <string.h>

// Create a new cache
CacheCore* cache_core_create(size_t capacity) {
    CacheCore* cache = (CacheCore*)malloc(sizeof(CacheCore));
    if (cache) {
        cache->data = malloc(capacity);
        cache->size = 0;
        cache->capacity = capacity;
        if (!cache->data) {
            free(cache);
            return NULL;
        }
    }
    return cache;
}

// Destroy the cache
void cache_core_destroy(CacheCore* cache) {
    if (cache) {
        free(cache->data);
        free(cache);
    }
}

// Set data in the cache
void cache_core_set_data(CacheCore* cache, void* data, size_t size) {
    if (cache && data && size <= cache->capacity) {
        memcpy(cache->data, data, size);
        cache->size = size; // Update the size after setting data
    }
}

// Get data from the cache
void* cache_core_get_data(CacheCore* cache) {
    return cache ? cache->data : NULL;
}

// Get the size of the cache
size_t cache_core_get_size(CacheCore* cache) {
    return cache ? cache->size : 0;
}

// Get the capacity of the cache
size_t cache_core_get_capacity(CacheCore* cache) {
    return cache ? cache->capacity : 0;
}

// Clear the cache
void cache_core_clear(CacheCore* cache) {
    if (cache) {
        memset(cache->data, 0, cache->capacity); // Clear the data
        cache->size = 0; // Reset the size
    }
}

// Resize the cache
int cache_core_resize(CacheCore* cache, size_t new_capacity) {
    if (!cache) return 0;
    
    void* new_data = realloc(cache->data, new_capacity);
    if (!new_data) return 0; // Memory allocation failed

    cache->data = new_data;
    cache->capacity = new_capacity;

    if (new_capacity < cache->size) {
        cache->size = new_capacity; // Adjust size if new capacity is smaller
    }

    return 1; // Resize successful
}

// Check if the cache is empty
int cache_core_is_empty(CacheCore* cache) {
    return cache ? (cache->size == 0) : 1; // Return 1 if empty, 0 otherwise
}

// Copy data from one cache to another
void cache_core_copy_data(CacheCore* dest, CacheCore* src) {
    if (dest && src) {
        cache_core_set_data(dest, cache_core_get_data(src), cache_core_get_size(src));
    }
}
