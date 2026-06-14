#include "i2e_pallette.h"

#include <math.h>

const unsigned char pallette[6][3] = {
    {0, 0, 0},
    {255, 255, 255},
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 0}
};

double pallette_linear[6][3];

double convert_srgb_to_linear(unsigned char srgb_value) {
    double srgb = srgb_value / 255.0;
    if (srgb <= 0.04045) {
        return srgb / 12.92;
    } else {
        return pow((srgb + 0.055) / 1.055, 2.4);
    }
}

void init_pallette() {
    for (int i = 0; i < 6; ++i) {
        pallette_linear[i][0] = convert_srgb_to_linear(pallette[i][0]);
        pallette_linear[i][1] = convert_srgb_to_linear(pallette[i][1]);
        pallette_linear[i][2] = convert_srgb_to_linear(pallette[i][2]);
    }
}
