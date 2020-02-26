#include "led_control.h"

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Arduino.h>

#include "utils.h"
#include "tracing.h"
#include "pacman.h"

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

#define __WJ 
#ifdef __WJ
  int led_control_matrix_pin = D6;
  int led_control_blink_pin = D4;
  int led_control_matrix_width = 16;
  int led_control_matrix_height = 16;
  int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;
#else
  int led_control_matrix_pin = 4;
  int led_control_blink_pin = 5;
  int led_control_matrix_width = 7;
  int led_control_matrix_height = 7;
  int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;
#endif

Adafruit_NeoMatrix led_matrix = Adafruit_NeoMatrix(led_control_matrix_width, led_control_matrix_height, led_control_matrix_pin,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);


void led_control_setup() {
  TRACE_IN();
  
  pinMode(led_control_matrix_pin, OUTPUT);
  led_control_set_led_matrix_size(led_control_matrix_width, led_control_matrix_height);
  led_matrix.begin();
  led_matrix.setTextWrap(false);
  led_matrix.setBrightness(255);
  
  TRACE_OUT();
}

void led_control_set_led_matrix_size(int width, int height) {
  TRACE_IN();
  
  led_control_matrix_width = width;
  led_control_matrix_height = height;
  led_control_matrix_size = width * height;

  led_matrix.begin();
  
  for (int c = 1; c <= led_control_matrix_width; c++) {
    for (int r = 1; r <= led_control_matrix_height; r++) {
      if ((c+r)%5 == 0) {
        led_matrix.drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(255, 255, 255));
      } else if ((c+r)%5 == 1) {
        led_matrix.drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(255, 0, 0));
      } else if ((c+r)%5 == 2) {
        led_matrix.drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 255, 0));
      } else if ((c+r)%5 == 3) {
        led_matrix.drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 0, 255));
      } else if ((c+r)%5 == 4) {
        led_matrix.drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 0, 0));
      }
    }
  }

  led_matrix.show();
  
  TRACE_OUT();
}

void led_control_set_cached_color(int column, int row, uint32_t color) {
  TRACE_IN();
  TRACE_VAR("column", String(column));
  TRACE_VAR("row", String(row));
  TRACE_VAR("color", String(color, HEX));
  
//  int i = xy_to_index(column, row);
//  led_control_led_colors[i] = color;

  TRACE_OUT();
}

uint32_t led_control_get_cached_color(int column, int row) {
  TRACE_IN();
  TRACE_VAR("column", String(column));
  TRACE_VAR("row", String(row));
  
//  int i = xy_to_index(column, row);
//
//  TRACE_OUT();
//  
//  return led_control_led_colors[i];
  return 0;
}

void led_control_update()
{
  TRACE_IN();

  update_pacman(led_matrix, led_control_matrix_width, led_control_matrix_height);
  
  TRACE_OUT();
}
