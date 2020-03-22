#include <Adafruit_NeoMatrix.h>
#include "colors.h"
#include "tracing.h"

int table[] = {0, 8, 20, 32, 44, 60, 80, 96, 116, 136, 156, 176, 192, 208, 224, 240, 255};

Colors::Colors(double brightness) {
  _brightness = brightness;
}

int scale_green(int green) {
  int lower = green/16;
  int upper = lower+1;
  int remainder = green-(lower*16);
  double scale = remainder / 15.0;
  return (1.0-scale)*table[lower] + scale*table[upper];
}

int16_t Colors::get_matrix_color(int r, int g, int b) {
  int scaled_r = _brightness*r;
  int scaled_g = _brightness*g;
  int scaled_b = _brightness*b;

  int corrected_g = scale_green(scaled_g);

  return Adafruit_NeoMatrix::Color(scaled_r,corrected_g,scaled_b);
}
