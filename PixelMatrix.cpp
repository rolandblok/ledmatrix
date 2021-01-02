
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

    uint8_t r = (uint8_t)(color >> 16);
    uint8_t g = (uint8_t)(color >> 8);
    uint8_t b = (uint8_t)color; 
    
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

/**
 * hue : 0 - 65535L
 * SAT : 0 - 255
 * VAL : 0 - 255
 */
uint32_t PixelMatrix::ColorHSV_32(uint16_t hue, uint8_t sat, uint8_t val) {
  //  uint32_t c32 = ColorHSV(hue, sat, val);
  //  c32 += 0xFF000000; // fill also a, because ADA forgot to.

  // new stolen method 1 
  uint32_t c32 = HSVtoRGB((360.0/65535.0)*(float)hue, (100.0 / 255.0)*(float)sat, (100.0 / 255.0)*(float)val);
  
  return c32;
}

/**
 * H(Hue): 0-360 degrees
 * S(Saturation): 0-100 percent
 * V(Value): 0-100 percent
 *  
 * R(Red): 0-255
 * G(Green): 0-255
 * B(Blue): 0-255
 * Converted to uint32 color
 * 
 * https://www.codespeedy.com/hsv-to-rgb-in-cpp/
 */
uint32_t PixelMatrix::HSVtoRGB(float H, float S,float V){
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        return this->Color(0,0,0);
    }
    float s = S/100.0;
    float v = V/100.0;
    float C = s*v;
    // float X = C*(1-abs(fmod(H/60.0, 2)-1));   //OMG : https://www.best-microcontroller-projects.com/arduino-absolute-value.html
    float hmod = fmod(H/60.0, 2.0)-1;
    float abshmod = (hmod < 0)  ? -hmod : hmod;
    float X = C*(1.0-abshmod);
    float m = v-C;
    float r,g,b;
    if(H >= 0.0 && H < 60.0){
        r = C,g = X,b = 0;
    }
    else if(H >= 60.0 && H < 120.0){
        r = X,g = C,b = 0.0;
    }
    else if(H >= 120.0 && H < 180.0){
        r = 0.0,g = C,b = X;
    }
    else if(H >= 180.0 && H < 240.0){
        r = 0.0,g = X,b = C;
    }
    else if(H >= 240.0 && H < 300.0){
        r = X,g = 0.0,b = C;
    }
    else{
        r = C,g = 0.0,b = X;
    }
    float R = (r+m)*255.0;
    float G = (g+m)*255.0;
    float B = (b+m)*255.0;
    uint32_t c32 = this->Color(R,G,B);
    c32 += 0xFF000000; 
    
    return c32;
    
}
