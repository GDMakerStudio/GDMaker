#include "file.h"

#include <stdlib.h>
#include <string.h>

#include "core/variant/variant.h"


File* file_new() {
    File *file = (File*)malloc(sizeof(File));
    file->file_handle = NULL;
    file->file_path = NULL;
    file->error = FILE_ERROR_NONE;
    return file;
}

bool file_open(File *file, const char *path, FileMode mode) {
    const char *mode_str = NULL;
    switch (mode) {
        case FILE_MODE_READ:
            mode_str = "r";
            break;
        case FILE_MODE_WRITE:
            mode_str = "w";
            break;
        case FILE_MODE_APPEND:
            mode_str = "a";
            break;
        case FILE_MODE_READ_WRITE:
            mode_str = "r+";
            break;
        default:
            file->error = FILE_ERROR_UNKNOWN;
            return false;
    }

    file->file_handle = fopen(path, mode_str);
    if (file->file_handle) {
        file->file_path = path;
        file->error = FILE_ERROR_NONE;
        return true;
    } else {
        file->error = FILE_ERROR_OPEN_FAILED;
        return false;
    }
}

void file_close(File *file) {
    if (file->file_handle) {
        fclose(file->file_handle);
        file->file_handle = NULL;
        file->error = FILE_ERROR_NONE;
    }
}

bool file_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

size_t file_read(File *file, void *buffer, size_t size) {
    if (file->file_handle) {
        size_t bytes_read = fread(buffer, 1, size, file->file_handle);
        if (bytes_read == 0 && ferror(file->file_handle)) {
            file->error = FILE_ERROR_READ_FAILED;
        }
        return bytes_read;
    }
    return 0;
}

size_t file_write(File *file, const void *buffer, size_t size) {
    if (file->file_handle) {
        size_t bytes_written = fwrite(buffer, 1, size, file->file_handle);
        if (bytes_written < size) {
            file->error = FILE_ERROR_WRITE_FAILED;
        }
        return bytes_written;
    }
    return 0;
}

long file_get_size(File *file) {
    if (file->file_handle) {
        long current_pos = ftell(file->file_handle);
        fseek(file->file_handle, 0, SEEK_END);
        long size = ftell(file->file_handle);
        fseek(file->file_handle, current_pos, SEEK_SET);
        return size;
    }
    return -1;
}

FileError file_get_error(File *file) {
    return file->error;
}

bool file_store_string(File *file, const char *string) {
    if (file->file_handle) {
        return file_write(file, string, strlen(string)) == strlen(string);
    }
    return false;
}

bool file_get_line(File *file, char *buffer, size_t size) {
    if (file->file_handle) {
        if (fgets(buffer, size, file->file_handle)) {
            return true;
        } else {
            file->error = FILE_ERROR_READ_FAILED;
        }
    }
    return false;
}

bool file_get_text(File *file, char *buffer, size_t size) {
    if (file->file_handle) {
        size_t bytes_read = fread(buffer, 1, size - 1, file->file_handle);
        buffer[bytes_read] = '\0'; // Null-terminate the string
        return bytes_read > 0;
    }
    return false;
}
