#ifndef UTILS
#define UTILS

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
  return get_nth_token(color, ',', 0).toInt()<<16 | get_nth_token(color, ',', 1).toInt()<<8 | get_nth_token(color, ',', 2).toInt();
}

int xy_to_index(int x, int y) {
  return (x-1)* LED_MATRIX_WIDTH + (y-1);
}

int xy_string_to_index(String x, String y) {
  return xy_to_index(x.toInt(), y.toInt());
}

// https://github.com/arendst/Tasmota/issues/1943
struct WifiAp { 
  String ssid;
  String pwd;
};

#endif
