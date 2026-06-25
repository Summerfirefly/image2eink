#ifndef I2E_UTILS_H
#define I2E_UTILS_H

#include <stddef.h>

extern const char *const bin_suffix;
extern const char *const preview_suffix;
extern const char *out_suffix;

int get_base_dir_name(const char *path, char *out_dirname, size_t dirname_length, char *out_basename,
                      size_t basename_length);
int is_filename_suffix_eq(const char *path, const char *extension);
char * generate_output_path(const char *path);

#endif /* I2E_UTILS_H */
