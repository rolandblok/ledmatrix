#include <Arduino.h>

#include "sprite.h"
#include "sprite_data.h"
#include "tracing.h"


typedef struct sprite {
  int no_frames;
  int width;
  int height;
  uint32_t* data;
} sprite;

#define WHITE_SPACE_BETWEEN_SPRITES 2

sprite sprites_glb[SPRITES_COUNT];
int    draw_all_width_glb ;
uint32_t _repl_org_color;
uint32_t _repl_new_color;
bool     _repl_color_enabled = false;

/**
 * I don't know how to create without function yet, easier import from piskel
 */
void sprite_create_sprites() {
  sprites_glb[SPRITES_RGBA].no_frames = RGBA_FRAME_COUNT;
  sprites_glb[SPRITES_RGBA].width     = RGBA_FRAME_WIDTH;
  sprites_glb[SPRITES_RGBA].height    = RGBA_FRAME_HEIGHT;
  sprites_glb[SPRITES_RGBA].data      = (uint32_t*)rgba_data;

  sprites_glb[SPRITES_DOG].no_frames = DOG_FRAME_COUNT;
  sprites_glb[SPRITES_DOG].width     = DOG_FRAME_WIDTH;
  sprites_glb[SPRITES_DOG].height    = DOG_FRAME_HEIGHT;
  sprites_glb[SPRITES_DOG].data      = (uint32_t*)dog_data;

  sprites_glb[SPRITES_PAKMEN].no_frames = PAKMEN_FRAME_COUNT;
  sprites_glb[SPRITES_PAKMEN].width     = PAKMEN_FRAME_WIDTH;
  sprites_glb[SPRITES_PAKMEN].height    = PAKMEN_FRAME_HEIGHT;
  sprites_glb[SPRITES_PAKMEN].data      = (uint32_t*)pakmen_data;

  sprites_glb[SPRITES_GHOST].no_frames = GHOST_FRAME_COUNT;
  sprites_glb[SPRITES_GHOST].width     = GHOST_FRAME_WIDTH;
  sprites_glb[SPRITES_GHOST].height    = GHOST_FRAME_HEIGHT;
  sprites_glb[SPRITES_GHOST].data      = (uint32_t*)ghost_data;

  sprites_glb[SPRITES_NUMBERS_3_5].no_frames = NUMBERS_3_5_FRAME_COUNT;
  sprites_glb[SPRITES_NUMBERS_3_5].width     = NUMBERS_3_5_FRAME_WIDTH;
  sprites_glb[SPRITES_NUMBERS_3_5].height    = NUMBERS_3_5_FRAME_HEIGHT;
  sprites_glb[SPRITES_NUMBERS_3_5].data      = (uint32_t*)numbers_3_5_data;

  sprites_glb[SPRITES_SPOOK_PAARS_7x7].no_frames = SPOOK_PAARS_7X7_FRAME_COUNT;
  sprites_glb[SPRITES_SPOOK_PAARS_7x7].width     = SPOOK_PAARS_7X7_FRAME_WIDTH;
  sprites_glb[SPRITES_SPOOK_PAARS_7x7].height    = SPOOK_PAARS_7X7_FRAME_HEIGHT;
  sprites_glb[SPRITES_SPOOK_PAARS_7x7].data      = (uint32_t*)spook_paars_7x7_data;

  sprites_glb[SPRITES_SPOOK_WIT_7x7].no_frames = SPOOK_WIT_7X7_FRAME_COUNT;
  sprites_glb[SPRITES_SPOOK_WIT_7x7].width     = SPOOK_WIT_7X7_FRAME_WIDTH;
  sprites_glb[SPRITES_SPOOK_WIT_7x7].height    = SPOOK_WIT_7X7_FRAME_HEIGHT;
  sprites_glb[SPRITES_SPOOK_WIT_7x7].data      = (uint32_t*)spook_wit_7x7_data;

  sprites_glb[SPRITES_PEKMEN_7x7].no_frames = PEKMEN_7X7_FRAME_COUNT;
  sprites_glb[SPRITES_PEKMEN_7x7].width     = PEKMEN_7X7_FRAME_WIDTH;
  sprites_glb[SPRITES_PEKMEN_7x7].height    = PEKMEN_7X7_FRAME_HEIGHT;
  sprites_glb[SPRITES_PEKMEN_7x7].data      = (uint32_t*)pekmen_7x7_data;

  

  draw_all_width_glb = WHITE_SPACE_BETWEEN_SPRITES * (SPRITES_COUNT-1);
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    draw_all_width_glb += sprites_glb[sprite_nr].width;
  }
  
}

void sprite_set_replacement_color(uint32_t org_color, uint32_t new_color) {
  _repl_org_color = org_color;
  _repl_new_color = new_color;
  _repl_color_enabled = true;
}
void sprite_disable_replacement_color() {
  _repl_color_enabled = false;
}


void sprite_draw_sprite(PixelMatrix *matrix, int16_t location_x, int16_t location_y, SPRITES_ENUM sprite_id, int16_t frame) {

  int width  = sprites_glb[sprite_id].width;
  int height = sprites_glb[sprite_id].height;
  int s      = width * height;
  int nof    = sprites_glb[sprite_id].no_frames;
 
  if ((frame < 0)) {
    Serial.println(" error : invalid frame request " + String(frame));
    frame = 0;
  } else {
    frame = (frame % nof);
  }
  
  for (int16_t h = 0; h < height; h++) {
    for (int16_t w = 0; w < width; w++) {
      uint32_t piksel = *(sprites_glb[sprite_id].data +(frame * s) + ((h*width)+w));
      if (_repl_color_enabled && (piksel == _repl_org_color)) {
        piksel = _repl_new_color;
      }
      
      int16_t alpha = matrix->get_alpha_color(piksel);
      if (alpha != 0) {
        matrix->draw_pixel(location_x+w, location_y+h, piksel);
      }
    }
  }

}


void sprite_update_sprites(PixelMatrix *matrix, Timer &timer, int matrix_width, int matrix_height, int y_offset) {
  TRACE_IN();

  int16_t time_in_period = timer.time_in_period(matrix_width + draw_all_width_glb + 1);
  int16_t location_x = -draw_all_width_glb + time_in_period;
  //Serial.println(" time_in_period " + String(time_in_period));
    
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    sprite_draw_sprite(matrix, location_x, y_offset, (SPRITES_ENUM)sprite_nr, time_in_period);
    location_x += sprites_glb[sprite_nr].width + WHITE_SPACE_BETWEEN_SPRITES;
  }
  

  TRACE_OUT();
}
