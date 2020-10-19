#include <Adafruit_NeoMatrix.h>
#include "colors.h"
#include "tracing.h"

int table[] = {0, 8, 20, 32, 44, 60, 80, 96, 116, 136, 156, 176, 192, 208, 224, 240, 255};

Colors::Colors(double brightness) {
  _brightness = brightness;
}

void replace_color() {

}

int scale_green(int green) {
  int lower = green/16;
  int upper = lower+1;
  int remainder = green-(lower*16);
  double scale = remainder / 15.0;
  return (1.0-scale)*table[lower] + scale*table[upper];
}

int16_t Colors::get_16b_color(int r, int g, int b) {
  
  int scaled_r = (int)(_brightness*(float)r);
  int scaled_g = (int)(_brightness*(float)g);
  int scaled_b = (int)(_brightness*(float)b);

  return Adafruit_NeoMatrix::Color(scaled_r, scaled_g, scaled_b);
}


int16_t Colors::get_16b_color_rgba(uint32 rgba, int * a) {
  //int x = (number >> (8*n)) & 0xff;
  int r = (rgba >> (8*0)) & 0xff ;
  int g = (rgba >> (8*1)) & 0xff ;
  int b = (rgba >> (8*2)) & 0xff ;
  *a = (rgba >> (8*3)) & 0xff ;
  
  //return Adafruit_NeoMatrix::Color(r,g,b);

  return this->get_16b_color(r,g,b);
}

String Colors::toString(uint32 rgba) {
  int r = (rgba >> (8*0)) & 0xff ;
  int g = (rgba >> (8*1)) & 0xff ;
  int b = (rgba >> (8*2)) & 0xff ;
  int a = (rgba >> (8*3)) & 0xff ;

  return toString(r,g,b,a);
}
String Colors::toString(int r, int g, int b, int a) {
  return "" + String(r,HEX) + "," + String(g,HEX) + "," + String(b,HEX) + "," + String(a,HEX);
}

uint32_t Colors::ColorHSV_32(uint16_t hue, uint8_t sat, uint8_t val) {
  uint32_t c32 = Adafruit_NeoMatrix::ColorHSV((uint16_t)(65535L * hue), sat, val);
  c32 += 0xFF000000; // fill also a, because ADA forgot to.
  return c32;
}
