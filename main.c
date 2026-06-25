#define _POSIX_C_SOURCE 200809L

#include <stb/stb_image.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "i2e_image_write.h"
#include "i2e_image_handler.h"
#include "i2e_pallette.h"
#include "i2e_utils.h"

int main(int argc, char *argv[]) {
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
    init_image_handler(width, height);

    for (int i = optind; i < argc; ++i) {
        if (is_filename_suffix_eq(argv[i], preview_suffix) || is_filename_suffix_eq(argv[i], bin_suffix)) {
            continue;
        }

        if (access(argv[i], F_OK) == 0) {
            if (is_filename_suffix_eq(argv[i], ".png") || is_filename_suffix_eq(argv[i], ".jpg") ||
                is_filename_suffix_eq(argv[i], ".jpeg") || is_filename_suffix_eq(argv[i], ".bmp")) {
                unsigned char *out_image_data = process_image(argv[i], auto_rotate, rotate_type);
                if (out_image_data == NULL) {
                    continue;
                }

                char *out_path = generate_output_path(argv[i]);

                if (out_bin_file) {
                    write_bincode_file(out_path, out_image_data, width, height);
                } else {
                    write_preview_file(out_path, out_image_data, width, height);
                }

                free(out_path);
                free(out_image_data);
                printf("[Done] %s\n", argv[i]);
            }
        }
    }

    return 0;
}
