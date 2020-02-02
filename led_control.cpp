#include "led_control.h"

#include "utils.h"
#include "tracing.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>


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
  int led_control_matrix_width = 3;
  int led_control_matrix_height = 3;
  int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;
#else
  int led_control_matrix_pin = 4;
  int led_control_blink_pin = 5;
  int led_control_matrix_width = 7;
  int led_control_matrix_height = 7;
  int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;
#endif

uint32_t *led_control_led_colors = NULL;
Adafruit_NeoPixel led_control_strip = Adafruit_NeoPixel();


int xy_to_index(int x, int y) {
  return (x-1)* led_control_matrix_width + (y-1);
}

int xy_string_to_index(String x, String y) {
  return xy_to_index(x.toInt(), y.toInt());
}

void led_control_setup() {
  TRACE_IN();
  
  pinMode(led_control_matrix_pin, OUTPUT);
  led_control_set_led_matrix_size(led_control_matrix_width, led_control_matrix_height);
  
  TRACE_OUT();
}

void led_control_set_led_matrix_size(int width, int height) {
  TRACE_IN();
  
  led_control_matrix_width = width;
  led_control_matrix_height = height;
  led_control_matrix_size = width * height;
  
  if (led_control_led_colors != NULL) {
    free(led_control_led_colors);
  }
  led_control_led_colors = (uint32_t*) malloc(width*height*sizeof(uint32_t));

  led_control_strip.updateLength(led_control_matrix_size);
  led_control_strip.setPin(led_control_matrix_pin);
  led_control_strip.begin();
  led_control_strip.show();
  
  for (int c = 1; c <= led_control_matrix_width; c++) {
    for (int r = 1; r <= led_control_matrix_height; r++) {
      int i = xy_to_index(c, r);
      if (c+r%2 == 0) {
        led_control_led_colors[i] = 0x00000000;
      } else {
        led_control_led_colors[i] = 0xFFFFFFFF;
      }
    }
  }

  led_control_copy_to_strip();
  
  TRACE_OUT();
}

void led_control_set_cached_color(int column, int row, uint32_t color) {
  TRACE_IN();
  TRACE_VAR("column", String(column));
  TRACE_VAR("row", String(row));
  TRACE_VAR("color", String(color, HEX));
  
  int i = xy_to_index(column, row);
  led_control_led_colors[i] = color;

  TRACE_OUT();
}

uint32_t led_control_get_cached_color(int column, int row) {
  TRACE_IN();
  TRACE_VAR("column", String(column));
  TRACE_VAR("row", String(row));
  
  int i = xy_to_index(column, row);

  TRACE_OUT();
  
  return led_control_led_colors[i];
}


void led_control_copy_to_strip() {
  TRACE_IN();
  
  for (int c = 1; c <= led_control_matrix_width; c++) {
    for (int r = 1; r <= led_control_matrix_height; r++) {
      int i = xy_to_index(c, r);
      led_control_strip.setPixelColor(i, led_control_led_colors[i]);
    }
  }
  led_control_strip.show();

  TRACE_OUT();
}
