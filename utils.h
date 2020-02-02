#ifndef UTILS
#define UTILS

#include <Arduino.h>

String get_nth_token(String data, char separator, int index);
String color_to_string(uint32_t color);
uint32_t string_to_color(String color);
int xy_to_index(int x, int y);
int xy_string_to_index(String x, String y);

#endif
