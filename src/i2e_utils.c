#include "i2e_utils.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

const char *const bin_suffix = ".bin";
const char *const preview_suffix = "_out.bmp";
const char *out_suffix = preview_suffix;

int get_base_dir_name(const char *path, char *out_dirname, size_t dirname_length, char *out_basename,
                      size_t basename_length) {
    if (path == NULL) {
        out_dirname[0] = '\0';
        out_basename[0] = '\0';
        return 0;
    }

    const char *last_slash = NULL;
    for (const char *p = path; *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') {
            last_slash = p;
        }
    }

    if (last_slash == NULL) {
        out_dirname[0] = '.';
        out_dirname[1] = '\0';
        strncpy(out_basename, path, basename_length - 1);
        out_basename[basename_length - 1] = '\0';
    } else {
        size_t dir_len = last_slash - path;
        if (dir_len >= dirname_length) {
            dir_len = dirname_length - 1;
        }

        strncpy(out_dirname, path, dir_len);
        out_dirname[dir_len] = '\0';

        strncpy(out_basename, last_slash + 1, basename_length - 1);
        out_basename[basename_length - 1] = '\0';
    }

    return 0;
}

int is_filename_suffix_eq(const char *path, const char *extension) {
    if (path == NULL || extension == NULL) {
        return 0;
    }

    size_t path_len = strlen(path);
    size_t extension_len = strlen(extension);

    if (path_len < extension_len) {
        return 0;
    }

    const char *path_ext = path + path_len - extension_len;
    return strcmp(path_ext, extension) == 0;
}

char * generate_output_path(const char *path) {
    if (path == NULL) {
        return NULL;
    }

    char *file_ext_start = strrchr(path, '.');
    size_t out_path_len = 0;
    size_t out_suffix_len = strlen(out_suffix);
    if (file_ext_start == NULL) {
        out_path_len = strlen(path) + out_suffix_len;
    } else {
        out_path_len = file_ext_start - path + out_suffix_len;
    }

    char *out_path = (char *)malloc(sizeof(char) * (out_path_len + 1));
    if (file_ext_start == NULL) {
        strncpy(out_path, path, strlen(path) + 1);
    } else {
        strncpy(out_path, path, file_ext_start - path + 1);
    }

    snprintf(out_path + out_path_len - out_suffix_len, out_suffix_len + 1, "%s", out_suffix);
    return out_path;
}
