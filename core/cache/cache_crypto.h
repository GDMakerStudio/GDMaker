#ifndef CACHE_CRYPTO_H
#define CACHE_CRYPTO_H

#include "core/cache/cache_core.h"

// Define the CacheCrypto structure
typedef struct {
    CacheCore* cache;
    char* key;
    CacheCore* original_data_cache; // Store original unencrypted data
} CacheCrypto;

// Function prototypes
CacheCrypto* cache_crypto_create(size_t size, const char* key);
void cache_crypto_destroy(CacheCrypto* cache);
void cache_crypto_encrypt(CacheCrypto* cache, void* data, size_t size);
void cache_crypto_decrypt(CacheCrypto* cache, void* data, size_t size);
void cache_crypto_clear(CacheCrypto* cache); // Clear encrypted cache data
void cache_crypto_set_key(CacheCrypto* cache, const char* new_key); // Set a new encryption key
const char* cache_crypto_get_key(CacheCrypto* cache); // Get the current encryption key
CacheCore* cache_crypto_get_original_data(CacheCrypto* cache); // Get the original data cache

#endif // CACHE_CRYPTO_H
