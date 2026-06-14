#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stb/stb_image_write.h>

#include "i2e_image_handler.h"
#include "i2e_options.h"
#include "i2e_utils.h"

int main(int argc, char *argv[]) {
    const char *out_suffix = "_out.bmp";

    for (int i = 1; i < argc; ++i) {
        if (is_filename_suffix_eq(argv[i], out_suffix)) {
            continue;
        }

        if (access(argv[i], F_OK) == 0) {
            if (is_filename_suffix_eq(argv[i], ".png") || is_filename_suffix_eq(argv[i], ".jpg") ||
                is_filename_suffix_eq(argv[i], ".jpeg") || is_filename_suffix_eq(argv[i], ".bmp")) {
                printf("Processing image: %s\n", argv[i]);
                double *image_data =
                    (double *)malloc(sizeof(double) * I2E_EINK_WIDTH * I2E_EINK_HEIGHT * 3);
                int err = load_image_data_linear(
                    argv[i],
                    image_data,
                    I2E_EINK_WIDTH,
                    I2E_EINK_HEIGHT
                );

                if (err != 0) {
                    free(image_data);
                    continue;
                }

                printf("Image loaded: %s\n", argv[i]);

                unsigned char *srgb_out_data = (unsigned char *)malloc(sizeof(unsigned char) * I2E_EINK_WIDTH * I2E_EINK_HEIGHT * 3);
                floyd_steinberg_dither_linear(image_data, srgb_out_data, I2E_EINK_WIDTH, I2E_EINK_HEIGHT);

                char *file_ext_start = strrchr(argv[i], '.');
                size_t out_path_len = 0;
                size_t out_suffix_len = strlen(out_suffix);
                if (file_ext_start == NULL) {
                    out_path_len = strlen(argv[i]) + out_suffix_len;
                } else {
                    out_path_len = file_ext_start - argv[i] + out_suffix_len;
                }

                char *out_path = (char *)malloc(sizeof(char) * (out_path_len + 1));
                if (file_ext_start == NULL) {
                    strlcpy(out_path, argv[i], strlen(argv[i]) + 1);
                } else {
                    strlcpy(out_path, argv[i], file_ext_start - argv[i] + 1);
                }

                snprintf(out_path + out_path_len - out_suffix_len, out_suffix_len + 1, "%s", out_suffix);

                stbi_write_bmp(out_path, I2E_EINK_WIDTH, I2E_EINK_HEIGHT, 3, srgb_out_data);
                free(image_data);
                free(srgb_out_data);

                printf("Image processed: %s\n", argv[i]);
            }
        }
    }

    return 0;
}
