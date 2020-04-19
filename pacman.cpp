#include <Arduino.h>

#include "pacman.h"
#include "pacman_data.h"
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
void pacman_create_sprites() {
  sprites_glb[SPRITES_RGBA].no_frames = RGBA_FRAME_COUNT;
  sprites_glb[SPRITES_RGBA].width     = RGBA_FRAME_WIDTH;
  sprites_glb[SPRITES_RGBA].height    = RGBA_FRAME_HEIGHT;
  sprites_glb[SPRITES_RGBA].data      = (uint32_t*)rgba_data;

  sprites_glb[SPRITES_PAKMEN].no_frames = PAKMEN_FRAME_COUNT;
  sprites_glb[SPRITES_PAKMEN].width     = PAKMEN_FRAME_WIDTH;
  sprites_glb[SPRITES_PAKMEN].height    = PAKMEN_FRAME_HEIGHT;
  sprites_glb[SPRITES_PAKMEN].data      = (uint32_t*)pakmen_data;

  sprites_glb[SPRITES_GHOST].no_frames = GHOST_FRAME_COUNT;
  sprites_glb[SPRITES_GHOST].width     = GHOST_FRAME_WIDTH;
  sprites_glb[SPRITES_GHOST].height    = GHOST_FRAME_HEIGHT;
  sprites_glb[SPRITES_GHOST].data      = (uint32_t*)ghost_data;

  draw_all_width_glb = WHITE_SPACE_BETWEEN_SPRITES * (SPRITES_COUNT-1);
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    draw_all_width_glb += sprites_glb[sprite_nr].width;
  }
  
  
}

#define ROUND(x) static_cast<uint8_t>(x+0.5)
#define ABS(x) ((x)>0?(x):-(x))

char *ghost_frame1[5] = {" BBB ",
                         "BBWBW",
                         "BBBBB",
                         "BBBBB",
                         "B B B"};

char *ghost_frame2[5] = {" BBB ",
                         "BBWBW",
                         "BBBBB",
                         "BBBBB",
                         "BB B "};

char *pacman_frame1[5] = {" YYY ",
                          "YYYYY",
                          "YYYYY",
                          "YYYYY",
                          " YYY "};

char *pacman_frame2[5] = {" YYY ",
                          "YYY  ",
                          "YY   ",
                          "YYY  ",
                          " YYY "};

char *cat[5] = {"W W W",
                "W RWR",
                "WWWWW",
                "WWWWW",
                "W W W"};

int16_t yellow = Adafruit_NeoMatrix::Color(255,255,0);
int16_t black = Adafruit_NeoMatrix::Color(0,0,0);
int16_t blue = Adafruit_NeoMatrix::Color(0,0,255);
int16_t red = Adafruit_NeoMatrix::Color(255,0,0);
int16_t white = Adafruit_NeoMatrix::Color(255,255,255);


void draw_sprite(Adafruit_NeoMatrix *matrix, Colors &colors, int16_t location_x, int16_t location_y, SPRITES_ENUM sprite_id, int16_t frame) {

  int width  = sprites_glb[sprite_id].width;
  int height = sprites_glb[sprite_id].height;
  int s      = width * height;
  int nof    = sprites_glb[sprite_id].no_frames;
 
  Serial.println(" frame " + String(frame) );
  if ((frame < 0)) {
    Serial.println(" error : invalid frame request " + String(frame));
    frame = 0;
  } else {
    frame = (frame % nof);
  }
  Serial.println("    frame " + String(frame) );
  
  for (int16_t h = 0; h < height; h++) {
    for (int16_t w = 0; w < width; w++) {
      uint32_t piksel = *(sprites_glb[sprite_id].data +(frame * s) + ((h*width)+w));
      
      matrix->drawPixel(location_x+w, location_y+h, colors.get_matrix_color_rgba(piksel));

    }
  }

}


void draw_from_characters(Adafruit_NeoMatrix *matrix, Colors &colors, int16_t location_x, int16_t location_y,
                          int16_t size_x, int16_t size_y, char **data) {
  for (int16_t y=0; y< size_y; y++) {
    for (int16_t x=0; x< size_x; x++) {
      switch (data[y][x]) {
        case 'X':
        case ' ':
          matrix->drawPixel(location_x+x, location_y+y, colors.get_matrix_color(0,0,0));
          break;
        case 'Y':
          matrix->drawPixel(location_x+x, location_y+y, colors.get_matrix_color(255,255,0));
          break;
        case 'B':
          matrix->drawPixel(location_x+x, location_y+y, colors.get_matrix_color(0,0,255));
          break;
        case 'W':
          matrix->drawPixel(location_x+x, location_y+y, colors.get_matrix_color(255,255,255));
          break;
        case 'R':
          matrix->drawPixel(location_x+x, location_y+y, colors.get_matrix_color(255,0,0));
          break;
      }
    }
  }
}



void draw_ghost(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y, int16_t ghost_color) {
  if (timer.is_in_period(1.0, 0.0, 0.5)) {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, ghost_frame1);
  } else {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, ghost_frame2);
  }
}


void draw_pacman(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y) {
  if (timer.is_in_period(2.0, 0.0, 1.0)) {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, pacman_frame1);
  } else {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, pacman_frame2);
  }
}

void draw_cat(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y) {
  draw_from_characters(matrix, colors, location_x, location_y, 5, 5, cat);
}


void update_pacman(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int matrix_width, int matrix_height) {
  TRACE_IN();

  int16_t time_in_period = timer.time_in_period(matrix_width + draw_all_width_glb + 1);
  int16_t location_x = -draw_all_width_glb + time_in_period;
  //Serial.println(" time_in_period " + String(time_in_period));
    
  for (int sprite_nr = (int)SPRITES_START; sprite_nr < (int)SPRITES_COUNT; sprite_nr++) {
    draw_sprite(matrix, colors, location_x, 8, (SPRITES_ENUM)sprite_nr, time_in_period);
    location_x += sprites_glb[sprite_nr].width + WHITE_SPACE_BETWEEN_SPRITES;
  }
  
  //int drawing_width = 21;
  //draw_sprite(matrix, colors, -drawing_width + time_in_period, 8, SPRITES_RGBA, time_in_period);
  //draw_sprite(matrix, colors, -drawing_width + time_in_period + 10, 8, SPRITES_GHOST, time_in_period);
  //draw_sprite(matrix, colors, -drawing_width + time_in_period + 20, 8, SPRITES_PAKMEN, time_in_period);
  
  //draw_cat(matrix, timer, colors, -drawing_width + time_in_period, 10);
  //draw_pacman(matrix, timer, colors, -drawing_width+ 5+3 + time_in_period, 10);
  //draw_ghost(matrix, timer, colors, -drawing_width+ 5+3+5+3 + time_in_period, 10, matrix->Color(0,0,255));

  TRACE_OUT();
}
