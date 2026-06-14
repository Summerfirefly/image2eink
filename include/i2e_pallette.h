#ifndef I2E_PALLETTE_H
#define I2E_PALLETTE_H

extern const unsigned char pallette[6][3];
extern double pallette_linear[6][3];

double convert_srgb_to_linear(unsigned char srgb_value);
void init_pallette();

#endif /* I2E_PALLETTE_H */
