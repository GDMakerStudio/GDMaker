#include "cache_crypto.h"


#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/io/file.h"
#include "error/error/error_macros.h"

static void xor_encrypt_decrypt(void* data, size_t size, const char* key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < size; i++) {
        ((char*)data)[i] ^= key[i % key_len];
    }
}

CacheCrypto* cache_crypto_create(size_t size, const char* key) {
    CacheCrypto* cache_crypto = (CacheCrypto*)malloc(sizeof(CacheCrypto));
    if (cache_crypto) {
        cache_crypto->cache = cache_core_create(size);
        if (!cache_crypto->cache) {
            free(cache_crypto);
            return NULL;
        }
        cache_crypto->original_data_cache = cache_core_create(size); // Store original data
        if (!cache_crypto->original_data_cache) {
            cache_core_destroy(cache_crypto->cache);
            free(cache_crypto);
            return NULL;
        }
        cache_crypto->key = strdup(key);
        if (!cache_crypto->key) {
            cache_core_destroy(cache_crypto->cache);
            cache_core_destroy(cache_crypto->original_data_cache);
            free(cache_crypto);
            return NULL;
        }
    }
    return cache_crypto;
}

// Destroy the encrypted cache
void cache_crypto_destroy(CacheCrypto* cache) {
    if (cache) {
        cache_core_destroy(cache->cache);
        cache_core_destroy(cache->original_data_cache); // Clean up original data cache
        free(cache->key);
        free(cache);
    }
}

// Encrypt data and store in the cache
void cache_crypto_encrypt(CacheCrypto* cache, void* data, size_t size) {
    if (cache && data && size <= cache_core_get_capacity(cache->cache)) {
        // Store original data before encrypting
        cache_core_copy_data(cache->original_data_cache, cache->cache);
        
        xor_encrypt_decrypt(data, size, cache->key);
        cache_core_set_data(cache->cache, data, size);
    }
}

// Decrypt data from the cache
void cache_crypto_decrypt(CacheCrypto* cache, void* data, size_t size) {
    if (cache && data && size <= cache_core_get_capacity(cache->cache)) {
        void* cached_data = cache_core_get_data(cache->cache);
        if (cached_data) {
            memcpy(data, cached_data, size);
            xor_encrypt_decrypt(data, size, cache->key);
        }
    }
}

// Clear the encrypted cache data
void cache_crypto_clear(CacheCrypto* cache) {
    if (cache) {
        cache_core_clear(cache->cache);
        cache_core_clear(cache->original_data_cache); // Clear original data as well
    }
}

// Set a new encryption key
void cache_crypto_set_key(CacheCrypto* cache, const char* new_key) {
    if (cache && new_key) {
        free(cache->key);
        cache->key = strdup(new_key);
    }
}

// Get the current encryption key
const char* cache_crypto_get_key(CacheCrypto* cache) {
    return cache ? cache->key : NULL;
}

// Get the original data cache
CacheCore* cache_crypto_get_original_data(CacheCrypto* cache) {
    return cache ? cache->original_data_cache : NULL;
}

// Generate random bytes
void cache_crypto_generate_random_bytes(unsigned char* buffer, int size) {
    srand((unsigned int)time(NULL)); // Seed the random number generator
    for (int i = 0; i < size; i++) {
        buffer[i] = rand() % 256; // Fill the buffer with random bytes
    }
}

void cache_crypto_generate_rsa(int size) {
    RSA* rsa = RSA_generate_key(size, RSA_F4, NULL, NULL);
    if (rsa == NULL) {
        fprintf(stderr, "Error generating RSA key\n");
        return;
    }

    // Save the public key
    File* pub_key_file = file_new();
    if (file_open(pub_key_filem "public_key.pem", FILE_MODE_WRITE_BLOCK) {
        PEM_write_RSA_PUBKEY(pub_key_file, rsa);
        file_close(pub_key_file);
    } else {
        ERROR_PRINT("saving public key\n");
    }

    File* priv_key_file = file_new();
    if (fopen(priv_key_file, "private_key.pem", FILE_MODE_WRITE)) {
        PEM_write_RSAPrivateKey(priv_key_file, rsa, NULL, NULL, 0, NULL, NULL);
        file_close(priv_key_file);
    } else {
        ERROR_PRINT("saving private key\n");
    }

    RSA_free(rsa);
}

void cache_crypto_generate_self_signed_certificate() {
    X509* x509 = X509_new();
    if (!x509) {
        fprintf(stderr, "Error creating X509 certificate\n");
        return;
    }

    X509_set_version(x509, 2); 
    ASN1_INTEGER* serial_number = ASN1_INTEGER_new();
    ASN1_INTEGER_set(serial_number, 1);
    X509_set_serialNumber(x509, serial_number);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!rsa) {
        fprintf(stderr, "Error generating RSA key for certificate\n");
        X509_free(x509);
        return;
    }
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);
    X509_set_pubkey(x509, pkey);

    // Set subject name
    X509_NAME* name = X509_NAME_new();
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8, (unsigned char *)"MySelfSignedCert", -1, -1, 0);
    X509_set_subject_name(x509, name);
    X509_set_issuer_name(x509, name); // Self-signed, so issuer is the same as subject

    if (X509_sign(x509, pkey, EVP_sha256()) == 0) {
        fprintf(stderr, "Error signing the certificate\n");
    } else {
        
        File* cert_file = file_new();
        if (file_open(cert_file, "self_signed_cert.pem", FILE_MODE_WRITE) {
            PEM_write_X509(cert_file, x509);
            file_close(cert_file);
        } else {
            ERROR_PRINT("self-signed is certificate\n");
        }
    }

    // finished and clear
    EVP_PKEY_free(pkey);
    X509_NAME_free(name);
    X509_free(x509);
}
