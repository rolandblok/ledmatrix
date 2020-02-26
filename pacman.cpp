#include <Arduino.h>

#include "pacman.h"
#include "tracing.h"

#define ROUND(x) static_cast<uint8_t>(x+0.5)
#define ABS(x) ((x)>0?(x):-(x))

void update_pacman(Adafruit_NeoMatrix &matrix, int width, int height) {
  TRACE_IN();

  unsigned long t = millis();
  double t_in_s = t / 1000.0;
  
  uint16_t c_x = (width-1)/2;
  uint16_t c_y = (height-1)/2;
  uint16_t r = c_x < c_y ? c_x : c_y;

  TRACE_VAR("c_x", String(c_x));
  TRACE_VAR("c_y", String(c_y));
  TRACE_VAR("r", String(r));

  uint16_t mouth_wide_open = width/4;

  TRACE_VAR("matrix.numPixels()", String(matrix.numPixels()));
  //matrix.fillScreen(matrix.Color(0,0,0));
  matrix.clear();
  
  matrix.fillCircle(c_x, c_x, r, matrix.Color(255, 255, 0));
  float unit_mouth = 0.5 + 0.5*sin(20.0 * t_in_s / PI);
  uint8_t mouth = ROUND(mouth_wide_open * unit_mouth);
  TRACE_VAR("unit_mouth", String(unit_mouth));
  TRACE_VAR("mouth", String(mouth));
  
  if (mouth > 0)
    matrix.fillTriangle(c_x, c_y, width-1, c_y-mouth, width-1, c_y+mouth, matrix.Color(0,0,0));

  matrix.show();

  TRACE_OUT();
}
