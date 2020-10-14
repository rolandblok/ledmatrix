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
    NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +  // Right : big pannel
//    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +  // LEFT : small pannel
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

void led_control_update(unsigned long current_time_ms)
{
  TRACE_IN();

  led_matrix->clear();
  
  Timer timer = Timer(5.0);
  
  //int potentiometer = analogRead(A0);
  //Colors colors = Colors(potentiometer/1024.0);
  Colors colors = Colors(1);

    
  String tijd = getStrTime();
  int16_t image_width = tijd.length()*6;

  if (led_control_matrix_aspect == 1) {

    double hue_clock_f = ((double)(current_time_ms % 10000)) / 10000.0;
    double hue_back_f = (hue_clock_f + 0.5);
    if (hue_back_f > 1.0)  (hue_back_f -= 1.0);

//    double hue_back_g = (hue_clock_f + 0.33);
//    if (hue_back_g > 1.0)  (hue_back_g -= 1.0);
//    double hue_back_b = (hue_clock_f + 0.66);
//    if (hue_back_b > 1.0)  (hue_back_b -= 1.0);


    uint32_t c = Adafruit_NeoMatrix::ColorHSV((uint16_t)(65535L * hue_clock_f), 255, 50);
    int dummy = 0;
    uint16_t cc = colors.get_matrix_color_rgba(c, &dummy);

//    Serial.print(" hue_back_f " + String(hue_clock_f) + " " + String(65535L*hue_clock_f) + " " + (uint16_t)(65535L * hue_clock_f));
//    Serial.println(" color " + String(Adafruit_NeoMatrix::ColorHSV((uint16_t)(65535L * hue_clock_f)),HEX));
    led_matrix->fillScreen(cc);
    //led_matrix->fillScreen(Adafruit_NeoMatrix::gamma32(Adafruit_NeoMatrix::Color(55*hue_clock_f,55*hue_back_g,55 * hue_back_b)));
    
     
//     uint32_t piksel = Adafruit_NeoMatrix::Color(255.0*hue_clock_f,255.0*hue_back_g,255.0 * hue_back_b);
//    Serial.println(" hue_clock_f " + String(hue_clock_f,4 ) + " color " + String(piksel,HEX));
  
//    for (int16_t h = 0; h < led_control_matrix_height; h++) {
//      for (int16_t w = 0; w < led_control_matrix_width; w++) {
//        led_matrix->drawPixel(w, h, piksel);
//      }
//    }
 
    
    // the big led pannel:
    div_t hour10 = div(hour(), 10);
    sprite_draw_sprite(led_matrix, colors, 1, 1, SPRITES_NUMBERS_3_7, hour10.quot);
    sprite_draw_sprite(led_matrix, colors, 5, 1, SPRITES_NUMBERS_3_7, hour10.rem);

    div_t minute10 = div(minute(), 10);
    sprite_draw_sprite(led_matrix, colors, 6, 7, SPRITES_NUMBERS_3_7, minute10.quot);
    sprite_draw_sprite(led_matrix, colors, 10, 7, SPRITES_NUMBERS_3_7, minute10.rem);


    
  } else if (led_control_matrix_aspect == 1) {
  // square led pannel 
    int16_t cursor_location = timer.get_location_back_and_forth(led_control_matrix_width, image_width, 1);
    led_matrix->setTextColor(colors.get_matrix_color(255,0,0));
    led_matrix->setCursor(cursor_location, 0);
    led_matrix->setTextSize(1);
    led_matrix->print(tijd);
  
    sprite_update_sprites(led_matrix, timer, colors, led_control_matrix_width, led_control_matrix_height, 8);
    
  }  else {
    // rectangle led pannel
    Timer swap_sprite_clock = Timer(20.0);
    led_matrix->setTextColor(colors.get_matrix_color(255,0,0));
    led_matrix->setCursor(0, 0);
    led_matrix->setTextSize(1);
    led_matrix->print(tijd);

    sprite_update_sprites(led_matrix, timer, colors, led_control_matrix_width, led_control_matrix_height, 0);

  }
  
  led_matrix->show();

  
  TRACE_OUT();
}
