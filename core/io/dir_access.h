#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    DIRECTORY_ERROR_NONE,
    DIRECTORY_ERROR_NOT_FOUND,
    DIRECTORY_ERROR_OPEN_FAILED,
    DIRECTORY_ERROR_CREATE_FAILED,
    DIRECTORY_ERROR_REMOVE_FAILED,
    DIRECTORY_ERROR_UNKNOWN
} DirectoryError;

typedef struct {
    char *path;
    void *dir_handle; // Platform-specific handle for the directory (e.g., DIR * for POSIX systems)
    DirectoryError error;
} Directory;

Directory* directory_new();
void directory_free(Directory *dir);
bool directory_open(Directory *dir, const char *path);
void directory_close(Directory *dir);
bool directory_exists(const char *path);
bool directory_is_empty(Directory *dir);
bool directory_read(Directory *dir, char *buffer, size_t size);
bool directory_get_current_dir(Directory *dir, char *buffer, size_t size);
bool directory_get_file_list(Directory *dir, char **file_list, size_t max_files);
bool directory_mkdir(const char *path);
bool directory_rmdir(const char *path);
bool directory_chdir(const char *path);
size_t directory_get_size(Directory *dir);
bool directory_rename(const char *from, const char *to);
void directory_list_dir_end(Directory *dir);
bool directory_list_dir_begin(Directory *dir, bool skip_navigational, bool skip_hidden);
size_t directory_get_space_left(const char *path);

#endif // DIRECTORY_H
