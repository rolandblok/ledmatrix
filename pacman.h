#ifndef PACMAN
#define PACMAN

#include <Adafruit_NeoMatrix.h>
#include "timer.h"
#include "colors.h"
#include "pacmen_data.h"

void update_pacman(Adafruit_NeoMatrix &matrix, Timer &timer, Colors &colors, int width, int height);

#define RGBA_FRAME_COUNT 1
#define RGBA_FRAME_WIDTH 8
#define RGBA_FRAME_HEIGHT 8

enum pix_obj_names {
  PIX_OBJ_START = -1,
  PAKMEN,
  GHOST,
  PIX_OBJ_COUNT
};

//static const pix_obj[PIX_OBJ_COUNT];





#endif 
