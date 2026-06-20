#ifndef I2E_IMAGE_HANDLER_H
#define I2E_IMAGE_HANDLER_H

#include <stddef.h>

extern int target_width;
extern int target_height;

void init_image_handler(int target_width, int target_height);
int floyd_steinberg_dither_linear(const double *const in_linear_data, unsigned char *const out_srgb_data, int width, int height);
double * load_image_data_linear(const char *path, int *width, int *height);
int rotate_image(unsigned char *const image_data, int *width, int *height, const int rotate_type);

#endif /* I2E_IMAGE_HANDLER_H */
