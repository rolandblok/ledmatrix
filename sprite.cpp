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

  sprites_glb[SPRITES_NUMBERS_3_7].no_frames = NUMBERS_3_7_FRAME_COUNT;
  sprites_glb[SPRITES_NUMBERS_3_7].width     = NUMBERS_3_7_FRAME_WIDTH;
  sprites_glb[SPRITES_NUMBERS_3_7].height    = NUMBERS_3_7_FRAME_HEIGHT;
  sprites_glb[SPRITES_NUMBERS_3_7].data      = (uint32_t*)numbers_3_7_data;

  draw_all_width_glb = WHITE_SPACE_BETWEEN_SPRITES * (SPRITES_COUNT-1);
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    draw_all_width_glb += sprites_glb[sprite_nr].width;
  }
  
}


void sprite_draw_sprite(Adafruit_NeoMatrix *matrix, Colors &colors, int16_t location_x, int16_t location_y, SPRITES_ENUM sprite_id, int16_t frame) {

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
      
      int a = 0;
      int16_t color = colors.get_matrix_color_rgba(piksel, &a);
      if (a != 0) {
        matrix->drawPixel(location_x+w, location_y+h, color);
      }
    }
  }

}


void sprite_update_sprites(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int matrix_width, int matrix_height, int y_offset) {
  TRACE_IN();

  int16_t time_in_period = timer.time_in_period(matrix_width + draw_all_width_glb + 1);
  int16_t location_x = -draw_all_width_glb + time_in_period;
  //Serial.println(" time_in_period " + String(time_in_period));
    
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    sprite_draw_sprite(matrix, colors, location_x, y_offset, (SPRITES_ENUM)sprite_nr, time_in_period);
    location_x += sprites_glb[sprite_nr].width + WHITE_SPACE_BETWEEN_SPRITES;
  }
  

  TRACE_OUT();
}
