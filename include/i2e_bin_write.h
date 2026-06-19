#ifndef I2E_BIN_OUT_H
#define I2E_BIN_OUT_H

int get_bincode_from_rgb(int r, int g, int b);
void write_bincode_file(const char *filepath, const unsigned char *image_data, int width, int height);

#endif // I2E_BIN_OUT_H
