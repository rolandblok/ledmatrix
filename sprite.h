#ifndef SPRITE
#define SPRITE

#include "PixelMatrix.h"
#include "timer.h"

enum SPRITES_ENUM {
  SPRITES_START = 0,
  SPRITES_RGBA = 0,
  SPRITES_DOG,
  SPRITES_PAKMEN,
  SPRITES_GHOST,
  SPRITES_NUMBERS_3_5,
  SPRITES_PEKMEN_7x7,
  SPRITES_SPOOK_PAARS_7x7,
  SPRITES_SPOOK_WIT_7x7,

  SPRITES_COUNT
};

void sprite_create_sprites();
void sprite_draw_sprite(PixelMatrix *matrix, int16_t location_x, int16_t location_y, SPRITES_ENUM sprite_id, int16_t frame);
void sprite_update_sprites(PixelMatrix *matrix, Timer &timer, int width, int height, int offset);

void sprite_set_replacement_color(uint32_t org_color, uint32_t new_color);
void sprite_disable_replacement_color();


#endif 
