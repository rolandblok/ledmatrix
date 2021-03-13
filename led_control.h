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
  LED_MODE_DIGIT,     //this is not for led pannel, but for digit clock mode.
  LED_MODE_OFF,
  
  LED_MODE_COUNT
};
static const char* LED_MODE_ENUM_STR[LED_MODE_COUNT] = 
{
  [LED_MODE_CLOCK]    = "clock mode",
  [LED_MODE_BREAKOUT] = "breakout mode",
  [LED_MODE_DIGIT]    = "digit mode",
  [LED_MODE_OFF]      = "off mode"
};


void led_control_set_mode(LED_MODE_ENUM led_mode);
void led_control_setup(int width, int height, boolean meander_mode);
void led_control_set_led_matrix_size(int width, int height);
void led_control_set_led_matrix_meander_mode(boolean meander_mode);
void led_control_set_brightness(int brightness);
void led_control_update(unsigned long current_time_ms);

void led_control_breakout_left();
void led_control_breakout_right();

#endif
