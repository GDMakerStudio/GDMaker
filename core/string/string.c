#include "string.h"




static void string_resize_buffer(String *str, size_t new_capacity) {
    str->data = (char *)realloc(str->data, new_capacity);
    str->capacity = new_capacity;
}

String *string_new() {
    String *str = (String *)malloc(sizeof(String));
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
    return str;
}

String *string_new_with_cstr(const char *cstr) {
    size_t len = strlen(cstr);
    String *str = (String *)malloc(sizeof(String));
    str->data = (char *)malloc(len + 1);
    strcpy(str->data, cstr);
    str->length = len;
    str->capacity = len + 1;
    return str;
}

String *string_new_with_string(const String *from) {
    return string_new_with_cstr(from->data);
}

String *string_copy(const String *other) {
    return string_new_with_cstr(other->data);
}

void string_free(String *str) {
    free(str->data);
    free(str);
}

void string_append(String *str, const char *cstr) {
    size_t len = strlen(cstr);
    if (str->length + len + 1 > str->capacity) {
        string_resize_buffer(str, str->length + len + 1);
    }
    strcpy(str->data + str->length, cstr);
    str->length += len;
}

void string_append_string(String *str, const String *other) {
    string_append(str, other->data);
}

void string_insert(String *str, size_t pos, const char *cstr) {
    size_t len = strlen(cstr);
    if (str->length + len + 1 > str->capacity) {
        string_resize_buffer(str, str->length + len + 1);
    }
    memmove(str->data + pos + len, str->data + pos, str->length - pos + 1);
    memcpy(str->data + pos, cstr, len);
    str->length += len;
}

void string_erase(String *str, size_t pos, size_t length) {
    if (pos + length <= str->length) {
        memmove(str->data + pos, str->data + pos + length, str->length - pos - length + 1);
        str->length -= length;
    }
}

void string_resize(String *str, size_t new_size) {
    if (new_size + 1 > str->capacity) {
        string_resize_buffer(str, new_size + 1);
    }
    str->length = new_size;
    str->data[new_size] = '\0';
}

void string_clear(String *str) {
    str->length = 0;
    if (str->data) {
        str->data[0] = '\0';
    }
}

bool string_empty(const String *str) {
    return str->length == 0;
}

char string_at(const String *str, size_t index) {
    if (index < str->length) {
        return str->data[index];
    }
    return '\0';
}

int string_compare(const String *str1, const String *str2) {
    return strcmp(str1->data, str2->data);
}

bool string_equals(const String *str1, const String *str2) {
    return string_compare(str1, str2) == 0;
}

const char *string_cstr(const String *str) {
    return str->data;
}

bool begins_with(const String *self, const char *text) {
    size_t text_len = strlen(text);
    if (text_len > self->length) {
        return false;
    }
    return strncmp(self->data, text, text_len) == 0;
}

bool ends_with(const String *self, const char *text) {
    size_t text_len = strlen(text);
    if (text_len > self->length) {
        return false;
    }
    return strncmp(self->data + self->length - text_len, text, text_len) == 0;
}

String *get_base_dir(const String *self) {
    String *base_dir = string_copy(self);
    char *last_slash = strrchr(base_dir->data, '/');
    if (last_slash) {
        size_t pos = last_slash - base_dir->data;
        string_resize(base_dir, pos);
    } else {
        string_clear(base_dir);
    }
    return base_dir;
}

String *get_slice(const String *self, const char *delimiter, int slice) {
    const char *start = self->data;
    const char *end;
    int current_slice = 0;

    while ((end = strstr(start, delimiter)) != NULL) {
        if (current_slice == slice) {
            size_t length = end - start;
            String *result = string_new();
            string_resize(result, length);
            strncpy(result->data, start, length);
            result->data[length] = '\0';
            return result;
        }
        start = end + strlen(delimiter);
        current_slice++;
    }

    if (current_slice == slice) {
        return string_new_with_cstr(start);
    }

    return string_new_with_cstr("");
}

String *get_file(const String *self) {
    const char *last_slash = strrchr(self->data, '/');
    if (last_slash) {
        return string_new_with_cstr(last_slash + 1);
    }
    return string_copy(self);
}

int get_slice_count(const String *self, const char *delimiter) {
    int count = 0;
    const char *start = self->data;

    while ((start = strstr(start, delimiter)) != NULL) {
        count++;
        start += strlen(delimiter);
    }

    return count + 1;
}

size_t size(const String *self) {
    return self->length;
}

void replace(String *self, const char *target, const char *replacement) {
    String *result = string_new();
    const char *start = self->data;
    const char *pos;

    size_t target_len = strlen(target);
    size_t replacement_len = strlen(replacement);

    while ((pos = strstr(start, target)) != NULL) {
        size_t segment_len = pos - start;
        char *segment = (char *)malloc(segment_len + 1);
        strncpy(segment, start, segment_len);
        segment[segment_len] = '\0';
        string_append(result, segment);
        string_append(result, replacement);
        free(segment);
        start = pos + target_len;
    }

    string_append(result, start);
    string_free(self);
    *self = *result;
    free(result);
}

String **split(const String *self, const char *delimiter, int *count) {
    *count = get_slice_count(self, delimiter);
    String **result = (String **)malloc(*count * sizeof(String *));
    const char *start = self->data;
    const char *end;
    int current_slice = 0;

    while ((end = strstr(start, delimiter)) != NULL) {
        size_t length = end - start;
        result[current_slice] = string_new();
        string_resize(result[current_slice], length);
        strncpy(result[current_slice]->data, start, length);
        result[current_slice]->data[length] = '\0';
        start = end + strlen(delimiter);
        current_slice++;
    }

    result[current_slice] = string_new_with_cstr(start);
    return result;
}

bool is_empty(const String *self) {
    return self->length == 0;
}

String *path_join(const String *self, const String *other) {
    // Check if self or other is empty
    if (is_empty(self)) {
        return string_copy(other);
    }
    if (is_empty(other)) {
        return string_copy(self);
    }

    // Determine if a separator is needed
    bool self_ends_with_slash = ends_with(self, "/");
    bool other_begins_with_slash = begins_with(other, "/");

    String *result = string_new_with_string(self);

    if (self_ends_with_slash && other_begins_with_slash) {
        // Both have slashes, remove one
        string_append(result, other->data + 1);
    } else if (!self_ends_with_slash && !other_begins_with_slash) {
        // Neither has a slash, add one
        string_append(result, "/");
        string_append(result, other->data);
    } else {
        // Only one has a slash, just concatenate
        string_append(result, other->data);
    }

    return result;
}
