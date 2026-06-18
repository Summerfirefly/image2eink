#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stb/stb_image_write.h>

#include "i2e_image_handler.h"
#include "i2e_pallette.h"
#include "i2e_utils.h"

int main(int argc, char *argv[]) {
    const char *out_suffix = "_out.bmp";
    int width = -1;
    int height = -1;
    int pallette_inited = 0;

    int opt;
    while ((opt = getopt(argc, argv, "w:h:p:")) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'p':
                init_pallette(optarg);
                pallette_inited = 1;
                break;
            case '?':
                return 1;
            default:
                break;
        }
    }

    if (width * height <= 0) {
        printf("Invalid target size: %d x %d\n", width, height);
        return 1;
    } else if (width < 0 && height < 0) {
        width = 800;
        height = 480;
    }

    if (!pallette_inited) {
        init_pallette("dwyrbg");
    }

    for (int i = optind; i < argc; ++i) {
        if (is_filename_suffix_eq(argv[i], out_suffix)) {
            continue;
        }

        if (access(argv[i], F_OK) == 0) {
            if (is_filename_suffix_eq(argv[i], ".png") || is_filename_suffix_eq(argv[i], ".jpg") ||
                is_filename_suffix_eq(argv[i], ".jpeg") || is_filename_suffix_eq(argv[i], ".bmp")) {
                double *image_data =
                    (double *)malloc(sizeof(double) * width * height * 3);
                if (load_image_data_linear(argv[i], image_data, width, height) != 0) {
                    free(image_data);
                    continue;
                }

                unsigned char *srgb_out_data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3);
                if (floyd_steinberg_dither_linear(image_data, srgb_out_data, width, height) != 0) {
                    printf("[Fail] %s\n", argv[i]);
                    free(image_data);
                    free(srgb_out_data);
                    continue;
                }

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

                stbi_write_bmp(out_path, width, height, 3, srgb_out_data);
                free(out_path);
                free(image_data);
                free(srgb_out_data);

                printf("[Done] %s\n", argv[i]);
            }
        }
    }

    return 0;
}
