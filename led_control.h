#ifndef LED_CONTROL
#define LED_CONTROL

#include <stdint.h>

#include "PixelMatrix.h"

extern int led_control_matrix_pin;
extern int led_control_blink_pin;
extern int led_control_matrix_width;
extern int led_control_matrix_height;
extern int led_control_matrix_size;
extern PixelMatrix *led_matrix;

enum LED_MODE_ENUM {
  LED_MODE_START = 0,
  LED_MODE_CLOCK = 0,
  LED_MODE_BREAKOUT,
  LED_MODE_OFF,
  
  LED_MODE_COUNT
};


void led_control_set_mode(LED_MODE_ENUM led_mode);
void led_control_setup(int width, int height);
void led_control_set_led_matrix_size(int width, int height);
void led_control_update(unsigned long current_time_ms);

void led_control_breakout_left();
void led_control_breakout_right();

#endif
