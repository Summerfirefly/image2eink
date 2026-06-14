#include "i2e_image_handler.h"

#include "stb_import.h"
#include <stddef.h>
#include <stdio.h>

unsigned char pallette[6][3] = {{0, 0, 0}, {255, 255, 255}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}};

double convert_srgb_to_linear(unsigned char srgb_value) {
    double srgb = srgb_value / 255.0;
    if (srgb <= 0.04045) {
        return srgb / 12.92;
    } else {
        return pow((srgb + 0.055) / 1.055, 2.4);
    }
}

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
        out_image_data[i] = convert_srgb_to_linear(255);
    }

    if (resize_w == target_width) {
        int start_offset = resize_w * (target_height - resize_h) / 2 * 3;
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
        printf("Failed to allocate memory for Floyed-Steinberg dither\n");
        return 1;
    }

    for (int i = 0; i < width * height * 3; ++i) {
        tmp_linear_data[i] = 0.0;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixel_data_pos = (width * y + x) * 3;
            double r = in_linear_data[pixel_data_pos] + tmp_linear_data[pixel_data_pos];
            double g = in_linear_data[pixel_data_pos + 1] + tmp_linear_data[pixel_data_pos + 1];
            double b = in_linear_data[pixel_data_pos + 2] + tmp_linear_data[pixel_data_pos + 2];
            int new_pixel_index = get_closest_color_index_linear(r, g, b);

            out_srgb_data[pixel_data_pos] = pallette[new_pixel_index][0];
            out_srgb_data[pixel_data_pos + 1] = pallette[new_pixel_index][1];
            out_srgb_data[pixel_data_pos + 2] = pallette[new_pixel_index][2];

            double r_err = r - convert_srgb_to_linear(pallette[new_pixel_index][0]);
            double g_err = g - convert_srgb_to_linear(pallette[new_pixel_index][1]);
            double b_err = b - convert_srgb_to_linear(pallette[new_pixel_index][2]);

            int tmp_x = x + 1;
            int tmp_y = y;
            if (tmp_x < width) {
                pixel_data_pos = (width * tmp_y + tmp_x) * 3;
                tmp_linear_data[pixel_data_pos] += r_err * 7 / 16;
                tmp_linear_data[pixel_data_pos + 1] += g_err * 7 / 16;
                tmp_linear_data[pixel_data_pos + 2] += b_err * 7 / 16;
            }

            tmp_x = x - 1;
            tmp_y = y + 1;
            if (tmp_x >= 0 && tmp_y < height) {
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

            tmp_x = x + 1;
            tmp_y = y + 1;
            if (tmp_x < width && tmp_y < height) {
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

int get_closest_color_index_linear(double r, double g, double b) {
    int closest_idx = -1;
    double closest_dis = -1.0;
    for (int i = 0; i < 6; ++i) {
        double dr = r - convert_srgb_to_linear(pallette[i][0]);
        double dg = g - convert_srgb_to_linear(pallette[i][1]);
        double db = b - convert_srgb_to_linear(pallette[i][2]);
        double distance = dr * dr + dg * dg + db * db;
        if (closest_dis < 0 || distance < closest_dis) {
            closest_dis = distance;
            closest_idx = i;
        }
    }

    return closest_idx;
}
