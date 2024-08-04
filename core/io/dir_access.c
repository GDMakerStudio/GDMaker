#include "dir_access.h"


#include <linux/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <arm-linux-gnueabihf/sys/vfs.h>
#include <errno.h>
#include <types.h>


Directory* directory_new() {
    Directory *dir = (Directory*)malloc(sizeof(Directory));
    dir->path = NULL;
    dir->dir_handle = NULL;
    dir->error = DIRECTORY_ERROR_NONE;
    return dir;
}

void directory_free(Directory *dir) {
    if (dir) {
        directory_close(dir);
        free(dir->path);
        free(dir);
    }
}

bool directory_open(Directory *dir, const char *path) {
    dir->dir_handle = opendir(path);
    if (dir->dir_handle) {
        dir->path = strdup(path);
        dir->error = DIRECTORY_ERROR_NONE;
        return true;
    } else {
        dir->error = DIRECTORY_ERROR_OPEN_FAILED;
        return false;
    }
}

void directory_close(Directory *dir) {
    if (dir->dir_handle) {
        closedir(dir->dir_handle);
        dir->dir_handle = NULL;
    }
}

bool directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

bool directory_is_empty(Directory *dir) {
    struct dirent *entry;
    if (dir->dir_handle) {
        entry = readdir(dir->dir_handle);
        if (entry) {
            // If we read an entry, the directory is not empty
            directory_close(dir);
            return false;
        }
    }
    return true;
}

bool directory_read(Directory *dir, char *buffer, size_t size) {
    struct dirent *entry;
    if (dir->dir_handle && (entry = readdir(dir->dir_handle))) {
        strncpy(buffer, entry->d_name, size - 1);
        buffer[size - 1] = '\0'; // Ensure null termination
        return true;
    }
    return false;
}

bool directory_get_current_dir(Directory *dir, char *buffer, size_t size) {
    if (getcwd(buffer, size)) {
        return true;
    }
    return false;
}

bool directory_get_file_list(Directory *dir, char **file_list, size_t max_files) {
    struct dirent *entry;
    size_t count = 0;

    if (dir->dir_handle) {
        while ((entry = readdir(dir->dir_handle)) && count < max_files) {
            // Skip the current and parent directory entries
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                file_list[count++] = strdup(entry->d_name); // Allocate memory for each file name
            }
        }
        return count > 0;
    }
    return false;
}

bool directory_mkdir(const char *path) {
    if (mkdir(path, 0755) == 0) { // Set the permission to 0755
        return true;
    }
    return false;
}

bool directory_rmdir(const char *path) {
    if (rmdir(path) == 0) {
        return true;
    }
    return false;
}

bool directory_chdir(const char *path) {
    if (chdir(path) == 0) {
        return true;
    }
    return false;
}

size_t directory_get_size(Directory *dir) {
    size_t total_size = 0;
    struct dirent *entry;
    struct statfs statbuf;

    if (dir->dir_handle) {
        while ((entry = readdir(dir->dir_handle))) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char full_path[512];
                snprintf(full_path, sizeof(full_path), "%s/%s", dir->path, entry->d_name);

                if (stat(full_path, &statbuf) == 0) {
                    total_size += statbuf.f_bsize;
                }
            }
        }
    }
    return total_size;
}

bool directory_rename(const char *from, const char *to) {
    if (rename(from, to) == 0) {
        return true;
    }
    return false;
}

void directory_list_dir_end(Directory *dir) {
    directory_close(dir);
}

bool directory_list_dir_begin(Directory *dir, bool skip_navigational, bool skip_hidden) {
    struct dirent *entry;
    if (dir->dir_handle) {
        while ((entry = readdir(dir->dir_handle))) {
            if ((skip_navigational && (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) ||
                (skip_hidden && entry->d_name[0] == '.')) {
                continue;
            }
            return true;
        }
    }
    return false;
}

size_t directory_get_space_left(const char *path) {
    struct statfs stat;
    if (statfs(path, &stat) != 0) {
        return 0;
    }
    return stat.f_bsize * stat.f_bavail;
}
