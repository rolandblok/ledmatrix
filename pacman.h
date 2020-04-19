#ifndef PACMAN
#define PACMAN

#include <Adafruit_NeoMatrix.h>
#include "timer.h"
#include "colors.h"

void pacman_create_sprites();
void update_pacman(Adafruit_NeoMatrix *matrix, Timer &timer, Colors &colors, int width, int height);






#endif 
