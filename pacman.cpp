#include <Arduino.h>

#include "pacman.h"
#include "tracing.h"

#define ROUND(x) static_cast<uint8_t>(x+0.5)
#define ABS(x) ((x)>0?(x):-(x))


void draw_ghost(Adafruit_NeoMatrix &matrix, int16_t location_x, int16_t location_y, int16_t ghost_color) {
  unsigned long t = millis();
  double t_in_s = t / 1000.0;

  int16_t eye_color = matrix.Color(255,255,255);
  int16_t erase_color = matrix.Color(0,0,0);

  TRACE_VAR("t_in_s", String(t_in_s));
  TRACE_VAR("location_x, location_y", String(location_x) + "," + String(location_y));

  matrix.fillRect(location_x, location_y, 5, 5, ghost_color);
  matrix.drawPixel(location_x, location_y, erase_color);
  matrix.drawPixel(location_x+4, location_y, erase_color);
  matrix.drawPixel(location_x+4, location_y, erase_color);
  matrix.drawPixel(location_x+2, location_y+1, eye_color);
  matrix.drawPixel(location_x+4, location_y+1, eye_color);
  if (fmod(6.0*t_in_s, 2.0) < 1.0) {
    //matrix.drawPixel(location_x, location_y+4, erase_color);
    matrix.drawPixel(location_x+2, location_y+4, erase_color);
    matrix.drawPixel(location_x+4, location_y+4, erase_color);
  } else {
    matrix.drawPixel(location_x+1, location_y+4, erase_color);
    matrix.drawPixel(location_x+3, location_y+4, erase_color);
  }
}

void draw_pacman(Adafruit_NeoMatrix &matrix, int16_t location_x, int16_t location_y) {
  unsigned long t = millis();
  double t_in_s = t / 1000.0;

  int16_t pacman_color = matrix.Color(255,255,0);
  int16_t erase_color = matrix.Color(0,0,0);

  matrix.fillRect(location_x, location_y, 5, 5, pacman_color);
  matrix.drawPixel(location_x, location_y, erase_color);
  matrix.drawPixel(location_x, location_y+4, erase_color);
  matrix.drawPixel(location_x+4, location_y, erase_color);
  matrix.drawPixel(location_x+4, location_y+4, erase_color);
  if (fmod(3.0*t_in_s, 2.0) < 1.0) {
    matrix.drawPixel(location_x+2, location_y+2, erase_color);
    matrix.drawPixel(location_x+3, location_y+1, erase_color);
    matrix.drawPixel(location_x+3, location_y+2, erase_color);
    matrix.drawPixel(location_x+3, location_y+3, erase_color);
    matrix.drawPixel(location_x+4, location_y+1, erase_color);
    matrix.drawPixel(location_x+4, location_y+2, erase_color);
    matrix.drawPixel(location_x+4, location_y+3, erase_color);
  }
}

void draw_cat(Adafruit_NeoMatrix &matrix, int16_t location_x, int16_t location_y) {
  unsigned long t = millis();
  double t_in_s = t / 1000.0;

  int16_t cat_color = matrix.Color(242,164,92);
  int16_t eye_color = matrix.Color(255,0,0);
  int16_t erase_color = matrix.Color(0,0,0);

  matrix.fillRect(location_x, location_y, 5, 5, cat_color);
  matrix.drawPixel(location_x+1, location_y, erase_color);
  matrix.drawPixel(location_x+3, location_y, erase_color);
  matrix.drawPixel(location_x+1, location_y+1, erase_color);
  matrix.drawPixel(location_x+1, location_y+4, erase_color);
  matrix.drawPixel(location_x+3, location_y+4, erase_color);
  matrix.drawPixel(location_x, location_y, cat_color);
  matrix.drawPixel(location_x+4, location_y+1, eye_color);
  matrix.drawPixel(location_x+2, location_y+1, eye_color);
}


void update_pacman(Adafruit_NeoMatrix &matrix, int width, int height) {
  TRACE_IN();

  unsigned long t = millis();
  double t_in_s = t / 1000.0;
  matrix.clear();

  if (0) {
    uint16_t c_x = (width-1)/2;
    uint16_t c_y = (height-1)/2;
    uint16_t r = c_x < c_y ? c_x : c_y;
  
    TRACE_VAR("c_x", String(c_x));
    TRACE_VAR("c_y", String(c_y));
    TRACE_VAR("r", String(r));
  
    uint16_t mouth_wide_open = width/4;
  
    TRACE_VAR("matrix.numPixels()", String(matrix.numPixels()));
    //matrix.fillScreen(matrix.Color(0,0,0));
    
    matrix.fillCircle(c_x, c_x, r, matrix.Color(255, 255, 0));
    float unit_mouth = 0.5 + 0.5*sin(20.0 * t_in_s / PI);
    uint8_t mouth = ROUND(mouth_wide_open * unit_mouth);
    TRACE_VAR("unit_mouth", String(unit_mouth));
    TRACE_VAR("mouth", String(mouth));
    
    if (mouth > 0)
      matrix.fillTriangle(c_x, c_y, width-1, c_y-mouth, width-1, c_y+mouth, matrix.Color(0,0,0));
  }
  else {
    int drawing_width = 21;
    draw_ghost(matrix, -drawing_width+16+(uint16_t)fmod(3.0*t_in_s, width+drawing_width+1), 9, matrix.Color(0,0,255));
    draw_cat(matrix, -drawing_width+(uint16_t)fmod(3.0*t_in_s, width+drawing_width+1), 9);
    draw_pacman(matrix, -drawing_width+8+(uint16_t)fmod(3.0*t_in_s, width+drawing_width+1), 9);
  }

  matrix.show();

  TRACE_OUT();
}
