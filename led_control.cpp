#include <stdlib.h>
#include "led_control.h"

#include <Arduino.h>
#include <TimeLib.h>

#include "PixelMatrix.h"
#include "utils.h"
#include "tracing.h"
#include "sprite.h"
#include "NTPtime.h"

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

int led_control_matrix_pin = D6;
int led_control_blink_pin = D4;
int led_control_matrix_width = 16;
int led_control_matrix_height = 16;
int led_control_matrix_aspect = 1;
int led_control_matrix_size = led_control_matrix_width * led_control_matrix_height;

PixelMatrix *led_matrix = NULL;  

static LED_MODE_ENUM led_control_led_mode = LED_MODE_CLOCK;
static int stick_pos = 1;
    


void led_control_set_mode(LED_MODE_ENUM led_mode) {
  led_control_led_mode = led_mode;
  Serial.println("set mode : " + String(led_mode));
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


void create_adafruit_object(int width, int height) {

  if (led_matrix != NULL) {
    delete led_matrix;
  }
  
  led_matrix = new PixelMatrix(width, height, led_control_matrix_pin, true
//    ,NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +  // Right : big pannel
//    ,NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +  // LEFT : small pannel
//    ,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG
  );

  led_matrix->setBrightness(255);
//  led_matrix->setTextWrap(false);
  led_matrix->begin();

  sprite_create_sprites();
}

void led_control_setup(int width, int height) {
  TRACE_IN();
  
  pinMode(led_control_matrix_pin, OUTPUT);
  if (width == 0)  { width = 16; }
  if (height == 0) { height = 16; }

  led_control_set_led_matrix_size(width, height);
  
  TRACE_OUT();
}

void led_control_set_led_matrix_size(int width, int height) {
  TRACE_IN();

  create_adafruit_object(width, height);
  
  led_control_matrix_width  = width;
  led_control_matrix_height = height;
  led_control_matrix_size   = width * height;
  led_control_matrix_aspect = width/height;

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

  led_matrix->clear();
    
  String tijd = getStrTime();
  int16_t image_width = tijd.length()*6;

  if (led_control_led_mode == LED_MODE_CLOCK) {

    led_matrix->clear();
  
    int dummy = 0;
    float hue_clock_f = ((float)(current_time_ms % 20000)) / 20000.0;

    uint32_t c32_clock = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_clock_f), 255, 255);
    sprite_set_replacement_color(0xFFFFFFFF , c32_clock);

    float hue_back_f = fmod(hue_clock_f + 0.5, 1.0);
    uint32_t c32_back = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_back_f), 255, 50);

    Serial.println(" f + " + String(hue_clock_f) + " b : " + String(hue_back_f));
    
    led_matrix->fill_screen(c32_back);
  
    // Create time
    div_t hour10 = div(hour(), 10);
    sprite_draw_sprite(led_matrix, 3, 2, SPRITES_NUMBERS_3_5, hour10.quot);
    sprite_draw_sprite(led_matrix, 7, 2, SPRITES_NUMBERS_3_5, hour10.rem);

    div_t minute10 = div(minute(), 10);
    sprite_draw_sprite(led_matrix, 6, 9, SPRITES_NUMBERS_3_5, minute10.quot);
    sprite_draw_sprite(led_matrix, 10, 9, SPRITES_NUMBERS_3_5, minute10.rem);

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
    static int minute_sprite_position = -6*(7+2); // 6 sprites, 7 pixels wide, 2 pixels space in between.
    static int hour_sprite_position = 15;         // they go left to right
    // =====MINUTE GHOSTS=====
    // activate ghosts at minute change
    if ((!ghost_min_active) && (minute() != last_minute)) {  
      ghost_min_active = true;
      last_minute = minute();
    }
    if (ghost_min_active) {
      if (minute_sprite_position == 15) {
        minute_sprite_position = -6*(7+2);    // 6 sprites, 7 pixels wide, 2 pixels space in between.
        ghost_min_active = false;
      }
   
      if (current_time_ms > (last_min_update_time_ms + STEP_TIME_MS)) {
        last_min_update_time_ms = current_time_ms;
        minute_sprite_position ++;
      }
      int sprite_index = current_time_ms / (STEP_TIME_MS);
      double hue_ghost_f = ((double)(current_time_ms % 1000)) / 1000.0;
      uint32_t c32_ghost = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_ghost_f), 255, 255);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 0, 8, SPRITES_PEKMEN_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 9, 8, SPRITES_SPOOK_PAARS_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 18, 8, SPRITES_SPOOK_BLAUW_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 27, 8, SPRITES_SPOOK_GROEN_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 36, 8, SPRITES_SPOOK_ROOD_7x7, sprite_index);
      sprite_set_replacement_color(0xFFFFFFFF , c32_ghost);
      sprite_draw_sprite(led_matrix, minute_sprite_position + 45, 8, SPRITES_SPOOK_WIT_7x7, sprite_index);
      sprite_disable_replacement_color();
    }
    // =====HOUR GHOSTS=====
    // activate ghosts at minute change
    if ((!ghost_hour_active) && (hour() != last_hour)) {  
      ghost_hour_active = true;
      last_hour = hour();
    }
    if (ghost_hour_active) {
      if (hour_sprite_position == -6*(7+2)) {   // 6 sprites, 7 pixels wide, 2 pixels space in between.
        hour_sprite_position = 15;
        ghost_hour_active = false;
      }
   
      if (current_time_ms > (last_hour_update_time_ms + STEP_TIME_MS)) {
        last_hour_update_time_ms = current_time_ms;
        hour_sprite_position --;
      }
      int sprite_index = current_time_ms / (STEP_TIME_MS);
      double hue_ghost_f = ((double)(current_time_ms % 1000)) / 1000.0;
      uint32_t c32_ghost = led_matrix->ColorHSV_32((uint16_t)(65535L * hue_ghost_f), 255, 255);
      sprite_set_replacement_color(0xFFFFFFFF , c32_ghost);
      sprite_draw_sprite(led_matrix, hour_sprite_position + 0, 1, SPRITES_SPOOK_WIT_7x7, sprite_index, true);
      sprite_disable_replacement_color();
      sprite_draw_sprite(led_matrix, hour_sprite_position + 9, 1, SPRITES_SPOOK_ROOD_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position + 18, 1, SPRITES_SPOOK_GROEN_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position + 27, 1, SPRITES_SPOOK_BLAUW_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position + 36, 1, SPRITES_SPOOK_PAARS_7x7, sprite_index, true);
      sprite_draw_sprite(led_matrix, hour_sprite_position + 45, 1, SPRITES_PEKMEN_7x7, sprite_index, true);
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

    
    
  } else if (led_control_led_mode == LED_MODE_OFF) {
  }
  
  
  led_matrix->show();

  
  TRACE_OUT();
}
