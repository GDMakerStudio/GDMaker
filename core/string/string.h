#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

String *string_new();
String *string_new_with_cstr(const char *cstr);
String *string_new_with_string(const String *from);
String *string_copy(const String *other);
void string_free(String *str);

void string_append(String *str, const char *cstr);
void string_append_string(String *str, const String *other);
void string_insert(String *str, size_t pos, const char *cstr);
void string_erase(String *str, size_t pos, size_t length);
void string_resize(String *str, size_t new_size);
void string_clear(String *str);
bool string_empty(const String *str);
char string_at(const String *str, size_t index);

int string_compare(const String *str1, const String *str2);
bool string_equals(const String *str1, const String *str2);

const char *string_cstr(const String *str);

bool begins_with(const String *self, const char *text);
bool ends_with(const String *self, const char *text);
String *get_base_dir(const String *self);
String *get_slice(const String *self, const char *delimiter, int slice);
String *get_file(const String *self);
int get_slice_count(const String *self, const char *delimiter);

size_t size(const String *self);
void replace(String *self, const char *target, const char *replacement);
String **split(const String *self, const char *delimiter, int *count);
bool is_empty(const String *self);
String *path_join(const String *self, const String *other);

#endif // STRING_H
