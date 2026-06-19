#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "i2e_bin_write.h"
#include "i2e_image_handler.h"
#include "i2e_pallette.h"
#include "i2e_utils.h"

int main(int argc, char *argv[]) {
    const char *bin_suffix = ".bin";
    const char *preview_suffix = "_out.bmp";
    const char *out_suffix = preview_suffix;

    int width = -1;
    int height = -1;
    int out_bin_file = 0;
    int rotate_type = 0;
    char *colour_set = "dwyrabg";
    bool auto_rotate = true;

    int opt;
    while ((opt = getopt(argc, argv, "w:h:p:br:")) != -1) {
        switch (opt) {
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 'p':
            colour_set = optarg;
            break;
        case 'b':
            out_suffix = bin_suffix;
            out_bin_file = 1;
            break;
        case 'r':
            rotate_type = atoi(optarg);
            auto_rotate = false;
            if (rotate_type < 0 || rotate_type > 3) {
                printf("Invalid rotate type: %d\n", rotate_type);
                return 1;
            }
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

    init_pallette(colour_set);

    for (int i = optind; i < argc; ++i) {
        if (is_filename_suffix_eq(argv[i], preview_suffix) || is_filename_suffix_eq(argv[i], bin_suffix)) {
            continue;
        }

        if (access(argv[i], F_OK) == 0) {
            if (is_filename_suffix_eq(argv[i], ".png") || is_filename_suffix_eq(argv[i], ".jpg") ||
                is_filename_suffix_eq(argv[i], ".jpeg") || is_filename_suffix_eq(argv[i], ".bmp")) {
                if (auto_rotate) {
                    int w, h, c;
                    if (stbi_info(argv[i], &w, &h, &c)) {
                        rotate_type = (width - height) * (w - h) < 0 ? 1 : 0;
                    } else {
                        rotate_type = 0;
                    }
                }

                int w = width, h = height;
                if (rotate_type == 1 || rotate_type == 3) {
                    // Rotate canvas first then rotate back when output
                    w = height;
                    h = width;
                }

                double *image_data = (double *)malloc(sizeof(double) * w * h * 3);
                if (load_image_data_linear(argv[i], image_data, w, h) != 0) {
                    free(image_data);
                    continue;
                }

                unsigned char *srgb_out_data = (unsigned char *)malloc(sizeof(unsigned char) * w * h * 3);
                if (floyd_steinberg_dither_linear(image_data, srgb_out_data, w, h) != 0) {
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

                rotate_image(srgb_out_data, &w, &h, rotate_type);
                if (out_bin_file) {
                    write_bincode_file(out_path, srgb_out_data, w, h);
                } else {
                    stbi_write_bmp(out_path, w, h, 3, srgb_out_data);
                }

                free(out_path);
                free(image_data);
                free(srgb_out_data);

                printf("[Done] %s\n", argv[i]);
            }
        }
    }

    return 0;
}
