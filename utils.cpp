#include "utils.h"

#include <Adafruit_NeoPixel.h>

String get_nth_token(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String color_to_string(uint32_t color) {
  return String((color>>16)&255) + "," + String((color>>8)&255) + "," + String(color&255);
}

uint32_t string_to_color(String color) {
  return Adafruit_NeoPixel::Color(get_nth_token(color, ',', 0).toInt(), get_nth_token(color, ',', 1).toInt(), get_nth_token(color, ',', 2).toInt());
}


/**
 * return a string that has traling spaces
 *   Number : number to be printed
 *   Digits : number of ditis to use
 *  Example : n = 40, d = 4 
 *   String : "  40"
 */
String trail1000print(int i){
  return trail1000print(i, false);
}

String trail1000print(int i, boolean use_zeros) {
  String filler_str = "";
  if (use_zeros) filler_str = "0";
  else           filler_str = " ";
  String s = "";
  if (i < 1000) s = s + filler_str;
  if (i < 100)  s = s + filler_str;
  if (i < 10)   s = s + filler_str;
  s = s + String(i);
  return s;
}

String trail10print(int i){
  return trail10print(i, false);
}

String trail10print(int i, boolean use_zeros){
  String filler_str = "";
  if (use_zeros) filler_str = "0";
  else           filler_str = " ";
  String s = "";
  if (i < 10)   s = s + filler_str;
  s = s + String(i);
  return s;
}
