#include "i2e_pallette.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#define COLOUR_NUM_MAX 6
rgb_t pallette[COLOUR_NUM_MAX];
rgb_linear_t pallette_linear[COLOUR_NUM_MAX];

int colour_num = 0;

void add_colour_to_pallette(int index, rgb_t colour) {
    if (index < COLOUR_NUM_MAX) {
        pallette[index].r = colour.r;
        pallette[index].g = colour.g;
        pallette[index].b = colour.b;

        pallette_linear[index].r = convert_srgb_to_linear(colour.r);
        pallette_linear[index].g = convert_srgb_to_linear(colour.g);
        pallette_linear[index].b = convert_srgb_to_linear(colour.b);
    }
}

double convert_srgb_to_linear(unsigned char srgb_value) {
    double srgb = srgb_value / 255.0;
    if (srgb <= 0.04045) {
        return srgb / 12.92;
    } else {
        return pow((srgb + 0.055) / 1.055, 2.4);
    }
}

void init_pallette(const char *colour_set) {
    for (size_t i = 0; i < strlen(colour_set); ++i) {
        switch (colour_set[i]) {
            case 'd':
                add_colour_to_pallette(colour_num++, (rgb_t){0, 0, 0});
                break;
            case 'w':
                add_colour_to_pallette(colour_num++, (rgb_t){255, 255, 255});
                break;
            case 'r':
                add_colour_to_pallette(colour_num++, (rgb_t){255, 0, 0});
                break;
            case 'g':
                add_colour_to_pallette(colour_num++, (rgb_t){0, 255, 0});
                break;
            case 'b':
                add_colour_to_pallette(colour_num++, (rgb_t){0, 0, 255});
                break;
            case 'y':
                add_colour_to_pallette(colour_num++, (rgb_t){255, 255, 0});
                break;
            default:
                break;
        }

        if (colour_num == COLOUR_NUM_MAX) {
            break;
        }
    }
}

int get_closest_color_index_linear(double r, double g, double b) {
    int closest_idx = -1;
    double closest_dis = -1.0;
    for (int i = 0; i < colour_num; ++i) {
        double dr = r - pallette_linear[i].r;
        double dg = g - pallette_linear[i].g;
        double db = b - pallette_linear[i].b;
        double distance = dr * dr + dg * dg + db * db;
        if (closest_dis < 0 || distance < closest_dis) {
            closest_dis = distance;
            closest_idx = i;
        }
    }

    return closest_idx;
}
