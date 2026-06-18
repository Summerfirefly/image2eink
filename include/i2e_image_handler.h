#ifndef I2E_IMAGE_HANDLER_H
#define I2E_IMAGE_HANDLER_H

#include <stddef.h>

int floyd_steinberg_dither_linear(const double *const in_linear_data, unsigned char *const out_srgb_data, int width, int height);
int load_image_data_linear(const char *path, double *out_image_data, size_t target_width, size_t target_height);

#endif /* I2E_IMAGE_HANDLER_H */
