#include <stdlib.h>
#include "led_control.h"

#include <Arduino.h>
#include <TimeLib.h>

#include "PixelMatrix.h"
#include "utils.h"
#include "tracing.h"
#include "sprite.h"
#include "NTPtime.h"

static int LEDS_PER_SEGMENT = 3;
void _draw_digitsegment(uint8_t digit, uint8_t number, uint32_t color);
void _init_led_control_set_led_matrix();

// for the generic variant, this must be define, but I know of no way to determine this compile time.
//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;


int led_control_matrix_pin = D6;
int led_control_blink_pin = D4;


int led_control_matrix_width = 16;
int led_control_matrix_height = 16;
int led_control_matrix_aspect = 1;
int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;
bool led_control_matrix_meander_mode = true;

PixelMatrix *led_matrix = NULL;  

static LED_MODE_ENUM led_control_led_mode = LED_MODE_CLOCK;
static int stick_pos = 1;
    


void led_control_set_mode(LED_MODE_ENUM led_mode) {
  led_control_led_mode = led_mode;
  Serial.println("set led mode : " + String(LED_MODE_ENUM_STR[led_mode]));
}

void led_control_breakout_left() {
  stick_pos -= 1;
  if (stick_pos < 1) {
    stick_pos = 1;
  }
  Serial.println(" breakout left : stick pos : " + String(stick_pos));
}
void led_control_breakout_right() {
  stick_pos += 1;
  if (stick_pos > (led_control_matrix_width-2)) {
    stick_pos = (led_control_matrix_width-2);
  }
  Serial.println(" breakout right : stick pos : " + String(stick_pos));
}


/**
 * Startup function
 */
void led_control_setup(int width, int height, boolean meander_mode) {

  TRACE_IN();

  pinMode(led_control_matrix_pin, OUTPUT);
  if (width == 0)  { width = 16; }
  if (height == 0) { height = 16; }

  led_control_matrix_width  = width;
  led_control_matrix_height = height;
  led_control_matrix_meander_mode = meander_mode;
  
  _init_led_control_set_led_matrix();
  
  TRACE_OUT();
}

void led_control_set_led_matrix_meander_mode(bool meander_mode){
  led_control_matrix_meander_mode = meander_mode;
  _init_led_control_set_led_matrix();
}

void led_control_set_led_matrix_size(int width, int height) {
  TRACE_IN();

  led_control_matrix_width  = width;
  led_control_matrix_height = height;

  _init_led_control_set_led_matrix();
}

/**
 * Initialize the led matrix, at startup or after resize or after meander mode change
 * private function
 */
void _init_led_control_set_led_matrix() {
  
  if (led_matrix != NULL) {
    delete led_matrix;
  }
  
  led_matrix = new PixelMatrix(led_control_matrix_width, led_control_matrix_height, led_control_matrix_pin, led_control_matrix_meander_mode
//    ,NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +  // Right : big pannel
//    ,NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +  // LEFT : small pannel
//    ,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG
  );

  led_matrix->setBrightness(255);
  led_matrix->begin();

  sprite_create_sprites();


  // draw init screen
  for (int c = 0; c < led_control_matrix_width/2; c++) {
    for (int r = 0; r < led_control_matrix_height/2; r++) {
      led_matrix->draw_pixel(c, r, led_matrix->Color(255, 255, 255));
    }
  }
  for (int c = led_control_matrix_width/2; c < led_control_matrix_width; c++) {
    for (int r = 0; r < led_control_matrix_height/2; r++) {
      led_matrix->draw_pixel(c, r, led_matrix->Color(255, 0, 0));
    }
  }
  for (int c = 0; c < led_control_matrix_width/2; c++) {
    for (int r = led_control_matrix_height/2 ; r < led_control_matrix_height; r++) {
      led_matrix->draw_pixel(c, r, led_matrix->Color(0, 255, 0));
    }
  }
  for (int c = led_control_matrix_width/2; c < led_control_matrix_width; c++) {
    for (int r = led_control_matrix_height/2 ; r < led_control_matrix_height; r++) {
      led_matrix->draw_pixel(c, r, led_matrix->Color(0, 0 , 255));
    }
  }

  led_matrix->show();
  delay(1000);
  
  TRACE_OUT();
}

void led_control_set_brightness(int brightness) {
  led_matrix->setBrightness(brightness);
}

void led_control_update(unsigned long current_time_ms)
{
  TRACE_IN();

    
  float hue_clock_f = ((float)(current_time_ms % 20000)) / 20000.0;

  uint32_t c32_clock = led_matrix->ColorHSV_32((uint16_t)(65535.0 * hue_clock_f), 255, 255);
  sprite_set_replacement_color(0xFFFFFFFF , c32_clock);

  float hue_back_f = fmod(hue_clock_f + 0.5, 1.0);
  uint32_t c32_back = led_matrix->ColorHSV_32((uint16_t)(65535.0 * hue_back_f), 255, 50);


  if (led_control_led_mode == LED_MODE_CLOCK) {

//    16
//      0123456789012345
//      ----------------
//    0|                | 
//    1|                | sssssss
//    2|   xxx xxx      | s     s
//    3|   x x x x      | s     s
//    4|   xxx xxx      | s     s
//    5|   x x x x      | s     s
//    6|   xxx xxx      | s     s
//    7|                | sssssss
//    8|                | sssssss 
//    9|      xxx xxx   | s     s   
//    0|      x x x x   | s     s  
//    1|      xxx xxx   | s     s 
//    2|      x x x x   | s     s  
//    3|      xxx xxx   | s     s 
//    4|                | sssssss 
//    5|                | 
//      ----------------
//      0123456789012345
//    14
//      01234567890123
//      --------------
//    0|              | sssssss
//    1|  Xxx Xxx     | s     s
//    2|  x x x x     | s     s
//    3|  xxx xxx     | s     s
//    4|  x x x x     | s     s
//    5|  xxx xxx     | s     s
//    6|              | sssssss
//    7|              | sssssss
//    8|     Xxx Xxx  | s     s   
//    9|     x x x x  | s     s  
//    0|     xxx xxx  | s     s 
//    1|     x x x x  | s     s  
//    2|     xxx xxx  | s     s 
//    3|              | sssssss 
//      --------------    
//      01234567890123
    
    //hard coded offsets : sorry
    int hour_quot_pos_x = 3;
    int hour_rem_pos_x  = 7;  
    int hour_pos_y = 2;
    int min_quot_pos_x = 6;
    int min_rem_pos_x  = 10;
    int min_pos_y  = 9;
    if (led_control_matrix_width == 14) {
      hour_quot_pos_x = 2;
      hour_rem_pos_x  = 6;
      min_quot_pos_x  = 5;
      min_rem_pos_x   = 9;
      hour_pos_y      = 1;
      min_pos_y       = 8; 
    } 
    

    led_matrix->clear();
  
    led_matrix->fill_screen(c32_back);
  
    // Create time
    div_t hour10 = div(hour(), 10);
    sprite_draw_sprite(led_matrix, hour_quot_pos_x, hour_pos_y, SPRITES_NUMBERS_3_5, hour10.quot);
    sprite_draw_sprite(led_matrix, hour_rem_pos_x, hour_pos_y, SPRITES_NUMBERS_3_5, hour10.rem);

    div_t minute10 = div(minute(), 10);
    sprite_draw_sprite(led_matrix, min_quot_pos_x, min_pos_y, SPRITES_NUMBERS_3_5, minute10.quot);
    sprite_draw_sprite(led_matrix, min_rem_pos_x, min_pos_y, SPRITES_NUMBERS_3_5, minute10.rem);

    sprite_disable_replacement_color();

    // ======================
    // draw pakmen and ghosts.
    static bool ghost_min_active = true;  // let's draws them at startup
    static bool ghost_hour_active = true; // let's draws them at startup
    static int  last_minute = minute();
    static int  last_hour = hour();
    static unsigned long STEP_TIME_MS = 250;
    static unsigned long last_min_update_time_ms = 0;
    static unsigned long last_hour_update_time_ms = 0;
    static int minute_sprite_position_x = -6*(7+2); // 6 sprites, 7 pixels wide, 2 pixels space in between.
    static int hour_sprite_position_x = led_control_matrix_width;         // they go left to right
    
    // =====MINUTE GHOSTS=====
    // activate ghosts at minute change
    if ((!ghost_min_active) && (minute() != last_minute)) {  
      ghost_min_active = true;
      last_minute = minute();
    }
    if (ghost_min_active) {
      if (minute_sprite_position_x == led_control_matrix_width) {
        minute_sprite_position_x = -6*(7+2);    // 6 sprites, 7 pixels wide, 2 pixels space in between.
        ghost_min_active = false;
      }
   
      if (current_time_ms > (last_min_update_time_ms + STEP_TIME_MS)) {
        last_min_update_time_ms = current_time_ms;
        minute_sprite_position_x ++;
      }
      int sprite_index = current_time_ms / (STEP_TIME_MS);
      double hue_ghost_f = ((double)(current_time_ms % 1000)) / 1000.0;
      uint32_t c32_ghost = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_ghost_f), 255, 255);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 0,  min_pos_y-1, SPRITES_PEKMEN_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 9,  min_pos_y-1, SPRITES_SPOOK_PAARS_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 18, min_pos_y-1, SPRITES_SPOOK_BLAUW_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 27, min_pos_y-1, SPRITES_SPOOK_GROEN_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 36, min_pos_y-1, SPRITES_SPOOK_ROOD_7x7, sprite_index);
      sprite_set_replacement_color(0xFFFFFFFF , c32_ghost);
      sprite_draw_sprite(led_matrix, minute_sprite_position_x + 45, min_pos_y-1, SPRITES_SPOOK_WIT_7x7, sprite_index);
      sprite_disable_replacement_color();
    }
    // =====HOUR GHOSTS=====
    // activate ghosts at minute change
    if ((!ghost_hour_active) && (hour() != last_hour)) {  
      ghost_hour_active = true;
      last_hour = hour();
    }
    if (ghost_hour_active) {
      if (hour_sprite_position_x == -6*(7+2)) {   // 6 sprites, 7 pixels wide, 2 pixels space in between.
        hour_sprite_position_x = led_control_matrix_width;
        ghost_hour_active = false;
      }
   
      if (current_time_ms > (last_hour_update_time_ms + STEP_TIME_MS)) {
        last_hour_update_time_ms = current_time_ms;
        hour_sprite_position_x --;
      }
      int sprite_index = current_time_ms / (STEP_TIME_MS);
      double hue_ghost_f = ((double)(current_time_ms % 1000)) / 1000.0;
      uint32_t c32_ghost = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_ghost_f), 255, 255);
      sprite_set_replacement_color(0xFFFFFFFF , c32_ghost);
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 0, hour_pos_y-1, SPRITES_SPOOK_WIT_7x7, sprite_index, true);
      sprite_disable_replacement_color();
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 9,  hour_pos_y-1, SPRITES_SPOOK_ROOD_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 18, hour_pos_y-1, SPRITES_SPOOK_GROEN_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 27, hour_pos_y-1, SPRITES_SPOOK_BLAUW_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 36, hour_pos_y-1, SPRITES_SPOOK_PAARS_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position_x + 45, hour_pos_y-1, SPRITES_PEKMEN_7x7, sprite_index, true);
    }

    led_matrix->show();

  } else if (led_control_led_mode == LED_MODE_DIGIT) {
    static int last_minute = -1;
    static int last_second = -1;
    // Create time
    div_t hour10 = div(hour(), 10);
    int hour_digit_2 = hour10.quot;
    int hour_digit_1 = hour10.rem;

    div_t minute10 = div(minute(), 10);
    int minute_digit_2 = minute10.quot;
    int minute_digit_1 = minute10.rem;

    if (last_minute != minute()) {
      Serial.println("digit time : " + String(hour_digit_2) + String(hour_digit_1) + " : " + String(minute_digit_2) + String(minute_digit_1));
      last_minute = minute();
    }
    
    div_t second10 = div(second(), 10);
    int second_digit_2 = second10.quot;
    int second_digit_1 = second10.rem;

    led_matrix->clear();
    _draw_digitsegment(0, second_digit_1, c32_clock);
    led_matrix->show();

    if (last_second != second()) {
      Serial.println(" " + String(last_second));
      last_second = second();
    }

    
  } else if (led_control_led_mode == LED_MODE_BREAKOUT) {
    static bool new_game = true;
    static int stick_width = 3;
    static unsigned long dots[3] = {0};
    static int ball_pos_x = 0; 
    static int ball_pos_y = 0; 
    static int ball_x_direction = 0;
    static int ball_y_direction = 0;
    static unsigned long STEP_TIME_MS = 200;
    static unsigned long last_update_time_ms = 0;

    led_matrix->clear();
  
    if (new_game) {
      new_game = false;
      last_update_time_ms = current_time_ms;
      stick_pos = led_control_matrix_width / 2 ;
      ball_pos_x = stick_pos;
      ball_pos_y = led_control_matrix_height - 2;
      ball_x_direction = 1;
      ball_y_direction = -1;
      
      dots[0] = ~0;
      dots[1] = ~0;
      dots[2] = ~0;
    }

    // update game
    if (current_time_ms > (last_update_time_ms + STEP_TIME_MS)) {
      last_update_time_ms = current_time_ms;
      int new_ball_pos_x = ball_pos_x + ball_x_direction;
      int new_ball_pos_y = ball_pos_y + ball_y_direction;
      //bounce left screen
      if (new_ball_pos_x < 0) {
        new_ball_pos_x = 1;
        ball_x_direction = -ball_x_direction;
      }
      //bounce right screen
      if (new_ball_pos_x == led_control_matrix_width) {
        new_ball_pos_x = led_control_matrix_width - 2;
        ball_x_direction = -ball_x_direction;
      }
      // bounce pad
      if (new_ball_pos_y > led_control_matrix_width - 2){
        // todo : add pad detection
        new_ball_pos_y = led_control_matrix_width - 3;
        ball_y_direction = -ball_y_direction;
      }
      // bounce and eat dots
      if (new_ball_pos_y < 3){
        if (dots[new_ball_pos_y] & (1 << new_ball_pos_x)) {
          dots[new_ball_pos_y] = dots[new_ball_pos_y] & ~(1<<new_ball_pos_x);  // remove dot
          new_ball_pos_x = ball_pos_x;
          new_ball_pos_y = ball_pos_y;
          ball_x_direction = -ball_x_direction;
          ball_y_direction = -ball_y_direction;
        }
      }
      if (new_ball_pos_y < 0) {
        new_ball_pos_y = 0;
        ball_y_direction = -ball_y_direction;
      }

      ball_pos_x = new_ball_pos_x;
      ball_pos_y = new_ball_pos_y;
      
    }

    // every 5 seconds, color cycle the dots, with offset :-)
    static unsigned long dots_cycle_time_long_ms = 0;
    static unsigned long dots_cycle_time_short_ms = 0;
    if ((current_time_ms - dots_cycle_time_long_ms) > 5000) {
      dots_cycle_time_long_ms = current_time_ms;
      dots_cycle_time_short_ms = current_time_ms;
    }


    // draw dots
    for (int row = 0; row < 3; row ++) {
      for (int pix = 0; pix < led_control_matrix_width; pix++) {
        if (dots[row] & (1 << pix)) {
          // get color cycle
          uint32_t c32_dot = led_matrix->Color(255, 255, 255);
          unsigned long dots_cycle_time_short_delta_ms = current_time_ms - dots_cycle_time_short_ms - (row+pix)*10;
          double hue_dot_f = (double)(dots_cycle_time_short_delta_ms / 2000.0);
          
          if ((hue_dot_f > 0) && (hue_dot_f < 1)) {
            c32_dot = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_dot_f), 255, 255);
          } 
          led_matrix->draw_pixel(pix, row, c32_dot);
        }
      }
    }
    // draw stick
    led_matrix->draw_pixel(stick_pos-1, led_control_matrix_height - 1, led_matrix->Color(255, 255, 255));
    led_matrix->draw_pixel(stick_pos,   led_control_matrix_height - 1, led_matrix->Color(255, 255, 255));
    led_matrix->draw_pixel(stick_pos+1, led_control_matrix_height - 1, led_matrix->Color(255, 255, 255));
    // draw ball
    led_matrix->draw_pixel(ball_pos_x, ball_pos_y, led_matrix->Color(255, 0, 0 ));

    
    led_matrix->show();
    
  } else if (led_control_led_mode == LED_MODE_OFF) {
  }
  
  

  
  TRACE_OUT();
}

//  -3-
// |   |
// 4   2
// |   |
//  -5-
// |   |
// 6   1
// |   |
//  -0-
void _draw_digitsegment(uint8_t digit, uint8_t number, uint32_t color) {
  //                                    0123456    0123456    0123456    0123456    0123456    0123456    0123456    0123456    0123456    0123456
      //                               0          1         2          3          4          5          6          7          8          9
  uint8_t segments_for_numbers[10] = {0b1111101, 0b0110000, 0b1011011, 0b1111010, 0b0110110, 0b1101110, 0b1101111, 0b0111000, 0b1111111, 0b1111110};
  
  uint8_t digit_led_offset = LEDS_PER_SEGMENT * 7 * digit;

  for (uint8_t seg = 0; seg < 7; seg++) {
    if ( segments_for_numbers[number] & (1<<seg)){
      for (uint8_t led = 0; led < LEDS_PER_SEGMENT; led++) {
         led_matrix->draw_pixel(digit_led_offset + LEDS_PER_SEGMENT*seg + led, 0, color);
      }
    }
  }
  
}
