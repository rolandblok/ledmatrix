#ifndef SPRITE
#define SPRITE

#include <Adafruit_NeoMatrix.h>
#include "timer.h"
#include "colors.h"

enum SPRITES_ENUM {
  SPRITES_START = 0,
  SPRITES_RGBA = 0,
  SPRITES_DOG,
  SPRITES_PAKMEN,
  SPRITES_GHOST,
  SPRITES_NUMBERS_3_7,

  SPRITES_COUNT
};

void sprite_create_sprites();
void sprite_draw_sprite(Adafruit_NeoMatrix *matrix, Colors &colors, int16_t location_x, int16_t location_y, SPRITES_ENUM sprite_id, int16_t frame);
void sprite_update_sprites(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int width, int height, int offset);



#endif 
