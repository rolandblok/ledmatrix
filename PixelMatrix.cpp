
#include "PixelMatrix.h"
#include "tracing.h"

PixelMatrix::PixelMatrix(uint16_t width, uint16_t height, uint16_t pin, bool meander_start_top_left ) 
      : Adafruit_NeoPixel( width*height, pin, NEO_GRB + NEO_KHZ800) {
  _width = width;
  _height = height;
  _one_if_right_to_left = meander_start_top_left ? 1 : 0;

}


void PixelMatrix::draw_pixel(int16_t  x, int16_t  y, uint32_t color) {

  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
    // only support meander
    // meander : swap the x-axis for the backgoign 
    if ( (y+_one_if_right_to_left) % 2 ) {
      x = _width -1 - x;
    }
  
    int16_t led_id = y * _width + x;

    // swap red and blue 
    uint8_t b = (uint8_t)(color >> 16);
    uint8_t g = (uint8_t)(color >> 8);
    uint8_t r = (uint8_t)color; 
    
    setPixelColor(led_id, r, g, b);
  }
}


int16_t PixelMatrix::get_alpha_color(uint32 rgba) {
  return  (rgba >> (8*3)) & 0xff;
}

void PixelMatrix::fill_screen(uint32_t color) {

  for (int led_id = 0; led_id < numPixels(); led_id++) {
    setPixelColor(led_id, color);
  }
}


//int16_t Colors::get_16b_color_rgba(uint32 rgba, int * a) {
//  //int x = (number >> (8*n)) & 0xff;
//  int r = (rgba >> (8*0)) & 0xff ;
//  int g = (rgba >> (8*1)) & 0xff ;
//  int b = (rgba >> (8*2)) & 0xff ;
//  *a = (rgba >> (8*3)) & 0xff ;
//  
//  //return Adafruit_NeoMatrix::Color(r,g,b);
//
//  return this->get_16b_color(r,g,b);
//}

String PixelMatrix::toString(uint32 rgba) {
  int r = (rgba >> (8*0)) & 0xff ;
  int g = (rgba >> (8*1)) & 0xff ;
  int b = (rgba >> (8*2)) & 0xff ;
  int a = (rgba >> (8*3)) & 0xff ;

  return toString(r,g,b,a);
}

String PixelMatrix::toString(int r, int g, int b, int a) {
  return "" + String(r,HEX) + "," + String(g,HEX) + "," + String(b,HEX) + "," + String(a,HEX);
}

uint32_t PixelMatrix::ColorHSV_32(uint16_t hue, uint8_t sat, uint8_t val) {
  uint32_t c32 = ColorHSV((uint16_t)(65535L * hue), sat, val);
  c32 += 0xFF000000; // fill also a, because ADA forgot to.
  return c32;
}
