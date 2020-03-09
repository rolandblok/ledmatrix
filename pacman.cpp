#include <Arduino.h>

#include "pacman.h"
#include "tracing.h"

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

int16_t yellow = Adafruit_NeoMatrix::Color(255,255,0);
int16_t black = Adafruit_NeoMatrix::Color(0,0,0);
int16_t blue = Adafruit_NeoMatrix::Color(0,0,255);
int16_t red = Adafruit_NeoMatrix::Color(255,0,0);
int16_t white = Adafruit_NeoMatrix::Color(255,255,255);

void draw_from_characters(Adafruit_NeoMatrix &matrix, Colors &colors, int16_t location_x, int16_t location_y,
                          int16_t size_x, int16_t size_y, char **data) {
  for (int16_t y=0; y< size_y; y++) {
    for (int16_t x=0; x< size_x; x++) {
      switch (data[y][x]) {
        case 'X':
        case ' ':
          matrix.drawPixel(location_x+x, location_y+y, colors.get_matrix_color(0,0,0));
          break;
        case 'Y':
          matrix.drawPixel(location_x+x, location_y+y, colors.get_matrix_color(255,255,0));
          break;
        case 'B':
          matrix.drawPixel(location_x+x, location_y+y, colors.get_matrix_color(0,0,255));
          break;
        case 'W':
          matrix.drawPixel(location_x+x, location_y+y, colors.get_matrix_color(255,255,255));
          break;
      }
    }
  }
}

void draw_ghost(Adafruit_NeoMatrix &matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y, int16_t ghost_color) {
  if (timer.is_in_period(1.0, 0.0, 0.5)) {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, ghost_frame1);
  } else {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, ghost_frame2);
  }
}


void draw_pacman(Adafruit_NeoMatrix &matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y) {
  if (timer.is_in_period(2.0, 0.0, 1.0)) {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, pacman_frame1);
  } else {
    draw_from_characters(matrix, colors, location_x, location_y, 5, 5, pacman_frame2);
  }
}

void draw_cat(Adafruit_NeoMatrix &matrix, Timer &timer, Colors &colors, int16_t location_x, int16_t location_y) {
  unsigned long t = millis();
  double t_in_s = t / 1000.0;

  int16_t cat_color = matrix.Color(242,164,92);
  int16_t eye_color = matrix.Color(255,0,0);
  int16_t erase_color = matrix.Color(0,0,0);

  matrix.fillRect(location_x, location_y, 5, 5, cat_color);
  matrix.drawPixel(location_x+1, location_y, erase_color);
  matrix.drawPixel(location_x+3, location_y, erase_color);
  matrix.drawPixel(location_x+1, location_y+1, erase_color);
  matrix.drawPixel(location_x+1, location_y+4, erase_color);
  matrix.drawPixel(location_x+3, location_y+4, erase_color);
  matrix.drawPixel(location_x, location_y, cat_color);
  matrix.drawPixel(location_x+4, location_y+1, eye_color);
  matrix.drawPixel(location_x+2, location_y+1, eye_color);
}


void update_pacman(Adafruit_NeoMatrix &matrix, Timer &timer, Colors &colors, int width, int height) {
  TRACE_IN();

  int width_of_drawing = 5 + 3 + 5 + 3 + 5;
  int drawing_width = 21;
  int16_t time_in_period = timer.time_in_period(width+drawing_width+1);
  draw_cat(matrix, timer, colors, -drawing_width + time_in_period, 9);
  draw_pacman(matrix, timer, colors, -drawing_width+ 5+3 + time_in_period, 9);
  draw_ghost(matrix, timer, colors, -drawing_width+ 5+3+5+3 + time_in_period, 9, matrix.Color(0,0,255));

  TRACE_OUT();
}
