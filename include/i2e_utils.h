#ifndef I2E_UTILS_H
#define I2E_UTILS_H

#include <stddef.h>

int get_base_dir_name(const char *path, char *out_dirname, size_t dirname_length, char *out_basename,
                      size_t basename_length);
int is_file_extension_eq(const char *path, const char *extension);

#endif /* I2E_UTILS_H */
