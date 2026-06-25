#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "i2e_config.h"
#include "i2e_image_write.h"
#include "i2e_image_handler.h"
#include "i2e_pallette.h"
#include "i2e_utils.h"

int main(int argc, char *argv[]) {
    i2e_config_t config;
    int file_args_start = parse_config_from_args(argc, argv, &config);

    init_pallette(config.colour_set);
    init_image_handler(config.output_width, config.output_height);

    for (int i = file_args_start; i < argc; ++i) {
        if (is_filename_suffix_eq(argv[i], preview_suffix) || is_filename_suffix_eq(argv[i], bin_suffix)) {
            continue;
        }

        if (access(argv[i], F_OK) == 0) {
            if (is_filename_suffix_eq(argv[i], ".png") || is_filename_suffix_eq(argv[i], ".jpg") ||
                is_filename_suffix_eq(argv[i], ".jpeg") || is_filename_suffix_eq(argv[i], ".bmp")) {
                unsigned char *out_image_data = process_image(argv[i], config.auto_rotate, config.rotate_type);
                if (out_image_data == NULL) {
                    continue;
                }

                char *out_path = generate_output_path(argv[i]);
                if (config.output_bin_file) {
                    write_bincode_file(out_path, out_image_data, config.output_width, config.output_height);
                } else {
                    write_preview_file(out_path, out_image_data, config.output_width, config.output_height);
                }

                free(out_path);
                free(out_image_data);
                printf("[Done] %s\n", argv[i]);
            }
        }
    }

    return 0;
}
