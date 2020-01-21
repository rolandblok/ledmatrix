#ifndef WEB_HANDLERS
#define WEB_HANDLERS

#include "utils.h"

#include "main_page.h"
void handle_main_page() {
  Serial.println("Main page requested");

  server.send(200, "text/html", main_page);
}

#include "scripts.h"
void handle_scripts() {
  Serial.println("Scripts requested");
  
  server.send(200, "​application/javascript", scripts);
}

void handle_get_data() {
  Serial.println("get_data requested");
  
  if (!server.hasArg("item"))
    return;

  String item = server.arg("item");

  if (item == "width") {
    server.send(200, "​text/plain", String(LED_MATRIX_WIDTH));
    return;
  } else if (item == "height") {
    server.send(200, "​text/plain", String(LED_MATRIX_HEIGHT));
    return;
  } else if (item == "color") {
    if (server.hasArg("x") && server.hasArg("y")) {
      String x = server.arg("x");
      String y = server.arg("y");
      int index = xy_string_to_index(x, y);
      server.send(200, "​text/plain", x + ";" + y + ";" + color_to_string(led_colors[index]));
      return;
    }
    else {
      String content = get_colors_of_all_leds();
      server.send(200, "​text/plain", content);
      return;
    }
  }
  server.send(400, "text/plain", "Unrecognized arguments to get_data");
}

String get_colors_of_all_leds() {
  String content = String();

  for (int x = 1; x <= LED_MATRIX_WIDTH; x++) {
    for (int y = 1; y <= LED_MATRIX_HEIGHT; y++) {
      int index = xy_to_index(x, y);
      content = content + String(x) + ";" + String(y) + ";" + color_to_string(led_colors[index]) + "\n";
    }
  }

  return content;
}

void handle_set_data() {
  Serial.println("set_data requested... ");
  
  if (!server.hasArg("item"))
    return;

  String item = server.arg("item");

  if (item == "color") {
    if (server.hasArg("x") && server.hasArg("y") && server.hasArg("value")) {
      int index = xy_string_to_index(server.arg("x"), server.arg("y"));
      uint32_t color = string_to_color(server.arg("value"));
      led_colors[index] = color;
      server.send(200, "​text/plain", "ok");
      Serial.println("led " + String(index) + " set to " + String(color, BIN));
      return;
    }
  }
  server.send(400, "text/plain", "Unrecognized arguments to set_data");
}

void handle_dump_led_data() {
  Serial.println("width = " + String(LED_MATRIX_WIDTH));
  Serial.println("height = " + String(LED_MATRIX_HEIGHT));
  for (int x = 1; x <= LED_MATRIX_WIDTH; x++) {
    for (int y = 1; y <= LED_MATRIX_HEIGHT; y++) {
      int index = xy_to_index(x,y);
      Serial.println("led x=" + String(x) + " y=" + String(y) + " index=" + String(index) + " is set to " + String(led_colors[index], BIN));
    }
  }
}

void handle_not_found() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

#endif
