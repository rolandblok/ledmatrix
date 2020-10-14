#ifndef LED_CONTROL
#define LED_CONTROL

#include <stdint.h>

extern int led_control_matrix_pin;
extern int led_control_blink_pin;
extern int led_control_matrix_width;
extern int led_control_matrix_height;
extern int led_control_matrix_size;

void led_control_setup(int width, int height);
void led_control_set_led_matrix_size(int width, int height);
void led_control_set_cached_color(int column, int row, uint32_t color);
uint32_t led_control_get_cached_color(int column, int row);
void led_control_update(unsigned long current_time_ms);

#endif
