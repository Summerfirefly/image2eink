#ifndef I2E_IMAGE_HANDLER_H
#define I2E_IMAGE_HANDLER_H

#include <stdbool.h>
#include <stddef.h>
#include "i2e_config.h"

void init_image_handler(int target_width, int target_height);
unsigned char * process_image(const char *filepath, bool auto_rotate, rotate_type_t rotate_type);
int floyd_steinberg_dither_linear(const double *const in_linear_data, unsigned char *const out_srgb_data, int width, int height);
double * load_image_data_linear(const char *path, int *width, int *height);
int rotate_image(unsigned char *const image_data, int *width, int *height, const rotate_type_t rotate_type);

#endif /* I2E_IMAGE_HANDLER_H */
