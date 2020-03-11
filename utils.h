#ifndef UTILS
#define UTILS

#include <Arduino.h>

String get_nth_token(String data, char separator, int index);
String color_to_string(uint32_t color);
uint32_t string_to_color(String color);
int xy_to_index(int x, int y);
int xy_string_to_index(String x, String y);

/**
 * return a string that has traling spaces
 *   Number : number to be printed
 *   Digits : number of ditis to use
 *  Example : n = 40, d = 4 
 *   String : "  40"
 */
String trail1000print(int i);
String trail1000print(int i, boolean use_zeros);
String trail10print(int i);
String trail10print(int i, boolean use_zeros);


#endif
