#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "core/variant/variant.h"

typedef struct {
    Variant *keys;
    Variant *values;
    size_t size;
    size_t capacity;
    void (*set)(struct Dictionary *, Variant *, Variant *);
    Variant *(*get)(struct Dictionary *, Variant *);
    void (*remove)(struct Dictionary *, Variant *);
    void (*clear)(struct Dictionary *);
    size_t (*find_key)(struct Dictionary *, Variant *);
    void (*destroy)(struct Dictionary *);
    int (*has)(struct Dictionary *, Variant *);
    Variant *(*get_with_default)(struct Dictionary *, Variant *, Variant *);
    int (*is_read_only)(struct Dictionary *);
    int (*is_empty)(struct Dictionary *);
    void (*merge)(struct Dictionary *, struct Dictionary *, int);
} Dictionary;

// Function prototypes
Dictionary dictionary_new();
void set_dict(Dictionary *dict, Variant *key, Variant *value);
Variant *get_dict(Dictionary *dict, Variant *key);
void remove_dict(Dictionary *dict, Variant *key);
void clear_dict(Dictionary *dict);
size_t find_key(Dictionary *dict, Variant *key);
void destroy_dict(Dictionary *dict);
int has_dict(Dictionary *dict, Variant *key);
Variant *get_with_default(Dictionary *dict, Variant *key, Variant *default_value);
int is_dict_read_only(Dictionary *dict);
int dictionary_is_empty(Dictionary *dict);
void merge_dict(Dictionary *dict, Dictionary *source, int overwrite);

#endif // DICTIONARY_H
