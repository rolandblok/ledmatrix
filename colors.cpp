#include <Adafruit_NeoMatrix.h>
#include "colors.h"

Colors::Colors(double brightness) {
  _brightness = brightness;
}

int16_t Colors::get_matrix_color(int r, int g, int b) {
  int scaled_r = _brightness*r;
  int scaled_g = _brightness*g;
  int scaled_b = _brightness*b;

  return Adafruit_NeoMatrix::Color(scaled_r,scaled_g,scaled_b);
}
