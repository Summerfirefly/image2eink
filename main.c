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
    for (int i = 1; i < argc; ++i) {
        if (access(argv[i], F_OK) == 0) {
            if (is_file_extension_eq(argv[i], ".png") || is_file_extension_eq(argv[i], ".jpg") ||
                is_file_extension_eq(argv[i], ".jpeg") || is_file_extension_eq(argv[i], ".bmp")) {
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

                size_t len = strlen(argv[i]) + strlen(".out.bmp") + 1;
                char *out_path = (char *)malloc(sizeof(char) * len);
                snprintf(out_path, len, "%s.out.bmp", argv[i]);

                stbi_write_bmp(out_path, I2E_EINK_WIDTH, I2E_EINK_HEIGHT, 3, srgb_out_data);
                free(image_data);
                free(srgb_out_data);

                printf("Image processed: %s\n", argv[i]);
            }
        }
    }

    return 0;
}
