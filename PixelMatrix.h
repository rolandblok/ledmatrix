#ifndef PIXELMATRIX_H
#define PIXELMATRIX_H

#include <Adafruit_NeoPixel.h>

 
class PixelMatrix : public Adafruit_NeoPixel {
  private:

      uint16_t _width;
      uint16_t _height;
      uint16_t _one_if_right_to_left;

  public:
      PixelMatrix(uint16_t width, uint16_t height, uint16_t pin, bool meander_start_top_left = true);
      void draw_pixel(int16_t x, int16_t y, uint32_t color);
      void fill_screen(uint32_t color);
      
      String toString(int r, int g, int b, int a);
      String toString(uint32 rgba);

      int16_t get_alpha_color(uint32 rgba);

      uint32_t ColorHSV_32(uint16_t hue, uint8_t sat=255, uint8_t val=255);

     
};
 
#endif
