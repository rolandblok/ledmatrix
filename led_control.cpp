#include <stdlib.h>
#include "led_control.h"

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Arduino.h>
#include <TimeLib.h>

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

static Adafruit_NeoMatrix *led_matrix = NULL;  

void create_adafruit_object(int width, int height) {

  if (led_matrix != NULL) {
    delete led_matrix;
  }
  
  led_matrix = new Adafruit_NeoMatrix(width, height, led_control_matrix_pin,
//    NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +  // Right : big pannel
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +  // LEFT : small pannel
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

  led_matrix->setBrightness(255);
  led_matrix->setTextWrap(false);
  led_matrix->begin();

  sprite_create_sprites();
}

void led_control_setup(int width, int height) {
  TRACE_IN();
  
  pinMode(A0, INPUT); 
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
  for (int c = 1; c <= led_control_matrix_width; c++) {
    for (int r = 1; r <= led_control_matrix_height; r++) {
      if ((c+r)%5 == 0) {
        led_matrix->drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(255, 255, 255));
      } else if ((c+r)%5 == 1) {
        led_matrix->drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(255, 0, 0));
      } else if ((c+r)%5 == 2) {
        led_matrix->drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 255, 0));
      } else if ((c+r)%5 == 3) {
        led_matrix->drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 0, 255));
      } else if ((c+r)%5 == 4) {
        led_matrix->drawPixel(c-1, r-1, Adafruit_NeoMatrix::Color(0, 0, 0));
      }
    }
  }

  led_matrix->show();
  delay(1000);
  
  TRACE_OUT();
}


void led_control_update(unsigned long current_time_ms)
{
  TRACE_IN();

  led_matrix->clear();
  
  Timer timer = Timer(5.0);
  
  int potentiometer = analogRead(A0);
  Colors colors = Colors(potentiometer/1024.0);
  //Colors colors = Colors(1);  // use if no potmeter available

    
  String tijd = getStrTime();
  int16_t image_width = tijd.length()*6;

  if (led_control_matrix_aspect == 1) {

    int dummy = 0;
    double hue_clock_f = ((double)(current_time_ms % 20000)) / 20000.0;
    double hue_back_f = fmod(hue_clock_f + 0.5, 1.0);

    uint32_t c32_clock = colors.ColorHSV_32((uint16_t)(65535L * hue_clock_f), 255, 255);
    sprite_set_replacement_color(0xFFFFFFFF , c32_clock);

    uint32_t c32_back = colors.ColorHSV_32((uint16_t)(65535L * hue_back_f), 255, 50);
    uint16_t c16_back = colors.get_16b_color_rgba(c32_back, &dummy);
    //Serial.println("front , " + String(hue_clock_f) + ", rgba, " + colors.toString(c32_clock) + ", back, " + String(hue_back_f) + ", rgba, " + colors.toString(c32_back) );
    led_matrix->fillScreen(c16_back);

    // Create time
    div_t hour10 = div(hour(), 10);
    sprite_draw_sprite(led_matrix, colors, 1, 1, SPRITES_NUMBERS_3_5, hour10.quot);
    sprite_draw_sprite(led_matrix, colors, 5, 1, SPRITES_NUMBERS_3_5, hour10.rem);

    div_t minute10 = div(minute(), 10);
    sprite_draw_sprite(led_matrix, colors, 6, 8, SPRITES_NUMBERS_3_5, minute10.quot);
    sprite_draw_sprite(led_matrix, colors, 10, 8, SPRITES_NUMBERS_3_5, minute10.rem);

    sprite_disable_replacement_color();

    // ======================
    // draw pakmen and ghosts.
    static bool ghost_active = false;
    static int  last_minute = minute();
    static unsigned long STEP_TIME_MS = 250;
    static unsigned long last_update_time_ms = 0;
    static int bottom_sprite_position = -3*(7+2);
    if (!ghost_active) {
      if (minute() != last_minute) {
        ghost_active = true;
        last_minute = minute();
      }
    }
    if (ghost_active) {
      if (bottom_sprite_position == 15) {
        bottom_sprite_position = -3*(7+2);
        ghost_active = false;
      }
   
      if (current_time_ms > (last_update_time_ms + STEP_TIME_MS)) {
        last_update_time_ms = current_time_ms;
        bottom_sprite_position ++;
      }
      int sprite_index = current_time_ms / (STEP_TIME_MS);
      double hue_ghost_f = ((double)(current_time_ms % 1000)) / 1000.0;
      uint32_t c32_ghost = colors.ColorHSV_32((uint16_t)(65535L * hue_ghost_f), 255, 255);
      sprite_set_replacement_color(0xFFFFFFFF , c32_ghost);
      sprite_draw_sprite(led_matrix, colors, bottom_sprite_position + 0, 7, SPRITES_PEKMEN_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, colors, bottom_sprite_position + 9, 7, SPRITES_SPOOK_PAARS_7x7, sprite_index);
      sprite_draw_sprite(led_matrix, colors, bottom_sprite_position + 18, 7, SPRITES_SPOOK_WIT_7x7, sprite_index);
      sprite_disable_replacement_color();
    }
    
    

    
  } else if (led_control_matrix_aspect == 1) {
  // square led pannel 
    int16_t cursor_location = timer.get_location_back_and_forth(led_control_matrix_width, image_width, 1);
    led_matrix->setTextColor(colors.get_16b_color(255,0,0));
    led_matrix->setCursor(cursor_location, 0);
    led_matrix->setTextSize(1);
    led_matrix->print(tijd);
  
    sprite_update_sprites(led_matrix, timer, colors, led_control_matrix_width, led_control_matrix_height, 8);
    
  }  else {
    // rectangle led pannel
    Timer swap_sprite_clock = Timer(20.0);
    led_matrix->setTextColor(colors.get_16b_color(255,0,0));
    led_matrix->setCursor(0, 0);
    led_matrix->setTextSize(1);
    led_matrix->print(tijd);

    sprite_update_sprites(led_matrix, timer, colors, led_control_matrix_width, led_control_matrix_height, 0);

  }

  


  
  
  led_matrix->show();

  
  TRACE_OUT();
}
