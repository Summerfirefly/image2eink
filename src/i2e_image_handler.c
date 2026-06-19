#include "i2e_image_handler.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i2e_pallette.h"
#include "stb_import.h"

int load_image_data_linear(const char *path, double *out_image_data, size_t target_width, size_t target_height) {
    if (out_image_data == NULL) {
        return 1;
    }

    int width, height, channels;
    unsigned char *image_data = stbi_load(path, &width, &height, &channels, 3);
    if (image_data == NULL) {
        return 1;
    }

    double radio = (double)width / (double)height;
    double target_radio = (double)target_width / (double)target_height;
    int resize_w = 0;
    int resize_h = 0;
    if (radio > target_radio) {
        resize_w = target_width;
        resize_h = (size_t)((double)target_width / (double)width * (double)height);
    } else {
        resize_w = (size_t)((double)target_height / (double)height * (double)width);
        resize_h = target_height;
    }

    unsigned char *temp_image_data =
        stbir_resize_uint8_srgb(image_data, width, height, 0, NULL, resize_w, resize_h, 0, STBIR_RGB);
    stbi_image_free(image_data);

    if (temp_image_data == NULL) {
        return 1;
    }

    for (size_t i = 0; i < target_width * target_height * 3; ++i) {
        out_image_data[i] = 1.0;
    }

    if (resize_w == target_width) {
        int start_offset = resize_w * ((target_height - resize_h) / 2) * 3;
        for (int i = 0; i < resize_w * resize_h * 3; ++i) {
            out_image_data[start_offset + i] = convert_srgb_to_linear(temp_image_data[i]);
        }
    } else if (resize_h == target_height) {
        int col_offset = (target_width - resize_w) / 2 * 3;
        for (int row = 0; row < resize_h; ++row) {
            for (int col = 0; col < resize_w * 3; ++col) {
                int offset = col_offset + row * target_width * 3;
                out_image_data[offset + col] = convert_srgb_to_linear(temp_image_data[row * resize_w * 3 + col]);
            }
        }
    }

    free(temp_image_data);
    return 0;
}

int floyd_steinberg_dither_linear(const double *const in_linear_data, unsigned char *const out_srgb_data, int width,
                                  int height) {
    double *tmp_linear_data = (double *)malloc(width * height * 3 * sizeof(double));
    if (tmp_linear_data == NULL) {
        return 1;
    }

    for (int i = 0; i < width * height * 3; ++i) {
        tmp_linear_data[i] = 0.0;
    }

    for (int y = 0; y < height; ++y) {
        int x_step = y % 2 == 0 ? 1 : -1;
        for (int x = x_step > 0 ? 0 : width - 1; x < width && x >= 0; x += x_step) {
            int pixel_data_pos = (width * y + x) * 3;
            double r = in_linear_data[pixel_data_pos] + tmp_linear_data[pixel_data_pos];
            double g = in_linear_data[pixel_data_pos + 1] + tmp_linear_data[pixel_data_pos + 1];
            double b = in_linear_data[pixel_data_pos + 2] + tmp_linear_data[pixel_data_pos + 2];
            int new_pixel_index = get_closest_color_index_linear(r, g, b);

            out_srgb_data[pixel_data_pos] = pallette[new_pixel_index].r;
            out_srgb_data[pixel_data_pos + 1] = pallette[new_pixel_index].g;
            out_srgb_data[pixel_data_pos + 2] = pallette[new_pixel_index].b;

            double r_err = r - pallette_linear[new_pixel_index].r;
            double g_err = g - pallette_linear[new_pixel_index].g;
            double b_err = b - pallette_linear[new_pixel_index].b;

            int tmp_x = x + x_step;
            int tmp_y = y;
            if (tmp_x < width && tmp_x >= 0) {
                pixel_data_pos = (width * tmp_y + tmp_x) * 3;
                tmp_linear_data[pixel_data_pos] += r_err * 7 / 16;
                tmp_linear_data[pixel_data_pos + 1] += g_err * 7 / 16;
                tmp_linear_data[pixel_data_pos + 2] += b_err * 7 / 16;
            }

            tmp_x = x - x_step;
            tmp_y = y + 1;
            if (tmp_x < width && tmp_x >= 0 && tmp_y < height) {
                pixel_data_pos = (width * tmp_y + tmp_x) * 3;
                tmp_linear_data[pixel_data_pos] += r_err * 3 / 16;
                tmp_linear_data[pixel_data_pos + 1] += g_err * 3 / 16;
                tmp_linear_data[pixel_data_pos + 2] += b_err * 3 / 16;
            }

            tmp_x = x;
            tmp_y = y + 1;
            if (tmp_y < height) {
                pixel_data_pos = (width * tmp_y + tmp_x) * 3;
                tmp_linear_data[pixel_data_pos] += r_err * 5 / 16;
                tmp_linear_data[pixel_data_pos + 1] += g_err * 5 / 16;
                tmp_linear_data[pixel_data_pos + 2] += b_err * 5 / 16;
            }

            tmp_x = x + x_step;
            tmp_y = y + 1;
            if (tmp_x < width && tmp_x >= 0 && tmp_y < height) {
                pixel_data_pos = (width * tmp_y + tmp_x) * 3;
                tmp_linear_data[pixel_data_pos] += r_err * 1 / 16;
                tmp_linear_data[pixel_data_pos + 1] += g_err * 1 / 16;
                tmp_linear_data[pixel_data_pos + 2] += b_err * 1 / 16;
            }
        }
    }

    free(tmp_linear_data);
    return 0;
}

int rotate_image(unsigned char *const image_data, int *width, int *height, const int rotate_type) {
    if (rotate_type == 0) {
        return 0;
    }

    if (rotate_type > 3 || rotate_type < 0) {
        return 1;
    }

    unsigned char *tmp_image_data = (unsigned char *)malloc((*width) * (*height) * 3);
    int nw, nh;
    for (int y = 0; y < *height; ++y) {
        for (int x = 0; x < *width; ++x) {
            int nx, ny;
            switch (rotate_type) {
                case 1:
                    nx = -y + (*height) - 1;
                    ny = x;
                    nw = *height;
                    nh = *width;
                    break;
                case 2:
                    nx = -x + (*width) - 1;
                    ny = -y + (*height) - 1;
                    nw = *width;
                    nh = *height;
                    break;
                case 3:
                    nx = y;
                    ny = -x + (*width) - 1;
                    nw = *height;
                    nh = *width;
                    break;
                default:
                    break;
            }

            int offset = (x + y * (*width)) * 3;
            int new_offset = (nx + ny * nw) * 3;
            tmp_image_data[new_offset] = image_data[offset];
            tmp_image_data[new_offset + 1] = image_data[offset + 1];
            tmp_image_data[new_offset + 2] = image_data[offset + 2];
        }
    }

    memcpy(image_data, tmp_image_data, (*width) * (*height) * 3);
    free(tmp_image_data);
    *width = nw;
    *height = nh;
    return 0;
}
