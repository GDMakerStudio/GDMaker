#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "core/object/ref_counted.h"
#include "core/error/error_list.h"


typedef enum {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_READ_WRITE,
    FILE_MODE_READ_BLOCK,
    FILE_MODE_UNKNOWN,
    FILE_MODE_READ_BLOCK
} FileMode;

typedef enum {
    FILE_ERROR_NONE,
    FILE_ERROR_NOT_FOUND,
    FILE_ERROR_OPEN_FAILED,
    FILE_ERROR_WRITE_FAILED,
    FILE_ERROR_READ_FAILED,
    FILE_ERROR_UNKNOWN
} FileError;

typedef struct File  {
    RefCounted *ref;
    FILE *file_handle;
    const char *file_path;
    FileError error;
} File;

File* file_new();
bool file_open(File *file, const char *path, FileMode mode);
void file_close(File *file);
bool file_exists(const char *path);
size_t file_read(File *file, void *buffer, size_t size);
size_t file_write(File *file, const void *buffer, size_t size);
long file_get_size(File *file);
FileError file_get_error(File *file);
bool file_store_string(File *file, const char *string);
bool file_get_line(File *file, char *buffer, size_t size);
bool file_get_text(File *file, char *buffer, size_t size);

#endif // FILE_H
