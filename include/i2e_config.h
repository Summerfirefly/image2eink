#ifndef I2E_CONFIG_H
#define I2E_CONFIG_H

#include <stdbool.h>

typedef enum rotate_type {
    NO_ROTATE = 0,

    CLOCKWISE_90 = 1,
    CLOCKWISE_180 = 2,
    CLOCKWISE_270 = 3,

    ANTI_CLOCKWISE_90 = CLOCKWISE_270,
    ANTI_CLOCKWISE_180 = CLOCKWISE_180,
    ANTI_CLOCKWISE_270 = CLOCKWISE_90
} rotate_type_t;

typedef struct i2e_config {
    int output_width;
    int output_height;

    const char *colour_set;

    bool output_bin_file;

    bool auto_rotate;
    rotate_type_t rotate_type;
} i2e_config_t;

int parse_config_from_args(int argc, char **argv, i2e_config_t *config);

#endif // I2E_CONFIG_H
