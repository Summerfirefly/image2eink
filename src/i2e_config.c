#define _POSIX_C_SOURCE 200809L

#include "i2e_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "i2e_utils.h"

int parse_config_from_args(int argc, char **argv, i2e_config_t *config) {
    config->output_width = -1;
    config->output_height = -1;
    config->output_bin_file = false;
    config->rotate_type = NO_ROTATE;
    config->auto_rotate = true;
    config->colour_set = "dwyrabg";

    int opt;
    while ((opt = getopt(argc, argv, "w:h:p:br:")) != -1) {
        int rotate_type;
        switch (opt) {
        case 'w':
            config->output_width = atoi(optarg);
            break;
        case 'h':
            config->output_height = atoi(optarg);
            break;
        case 'p':
            config->colour_set = optarg;
            break;
        case 'b':
            out_suffix = bin_suffix;
            config->output_bin_file = true;
            break;
        case 'r':
            rotate_type = (rotate_type_t)atoi(optarg);
            if (rotate_type < 0 || rotate_type > 3) {
                printf("Invalid rotate type: %d\n", rotate_type);
                return 1;
            }
            config->rotate_type = rotate_type;
            config->auto_rotate = false;
            break;
        case '?':
            return 1;
        default:
            break;
        }
    }

    if (config->output_width * config->output_height <= 0) {
        printf("Invalid target size: %d x %d\n", config->output_width, config->output_height);
        return 1;
    } else if (config->output_width < 0 && config->output_height < 0) {
        config->output_width = 800;
        config->output_height = 480;
    }

    return optind;
}
