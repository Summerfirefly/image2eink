#ifndef I2E_PALLETTE_H
#define I2E_PALLETTE_H

#define COLOUR_NUM_MAX 6

typedef struct rgb_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_t;

typedef struct rgb_linear_t {
    double r;
    double g;
    double b;
} rgb_linear_t;

extern rgb_t pallette[COLOUR_NUM_MAX];
extern rgb_linear_t pallette_linear[COLOUR_NUM_MAX];
extern int pallette_bincode[COLOUR_NUM_MAX];

double convert_srgb_to_linear(unsigned char srgb_value);
int get_closest_color_index_linear(double r, double g, double b);
int get_colour_num(void);
void init_pallette(const char *colour_set);

#endif /* I2E_PALLETTE_H */
