#include "i2e_bin_write.h"

#include <stdio.h>
#include "i2e_pallette.h"

int get_bincode_from_rgb(int r, int g, int b) {
    int i = 0;
    for (i = 0; i < get_colour_num(); ++i) {
        if (pallette[i].r == r && pallette[i].g == g && pallette[i].b == b) {
            return pallette_bincode[i];
        }
    }

    return -1;
}

void write_bincode_file(const char *filepath, const unsigned char *image_data, int width, int height) {
    if (filepath == NULL || image_data == NULL) {
        return;
    }

    FILE *binfile = fopen(filepath, "wb+");
    if (binfile == NULL) {
        return;
    }

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const unsigned char *data_index = image_data + ((i + j * width) * 3);
            int bincode = get_bincode_from_rgb(data_index[0], data_index[1], data_index[2]);
            fwrite(&bincode, 1, 1, binfile);
        }
    }

    fflush(binfile);
    fclose(binfile);
}
