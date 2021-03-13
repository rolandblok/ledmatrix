#include "web_handlers.h"

#include "utils.h"
#include "led_control.h"
#include "tracing.h"
#include "non_volatile_data.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
WiFiServer stream_server(21);
WiFiClient stream_client;

void setup_web_server() {
  TRACE_IN();

  server.on("/", handle_main_page);
  server.on("/scripts.js", handle_scripts);
  server.on("/get_data", handle_get_data);
  server.on("/set_data", handle_set_data);
  
  server.on("/right",    [](){led_control_breakout_right(); server.send(200, "​text/plain", "ok");});
  server.on("/left",     [](){led_control_breakout_left(); server.send(200, "​text/plain", "ok");});
  server.on("/breakout", [](){led_control_set_mode(LED_MODE_BREAKOUT); server.send(200, "​text/plain", "ok");});
  server.on("/clock",    [](){led_control_set_mode(LED_MODE_CLOCK); server.send(200, "​text/plain", "ok");});
  server.on("/digit",    [](){led_control_set_mode(LED_MODE_DIGIT); server.send(200, "​text/plain", "ok");});
  server.on("/stream",   [](){led_control_set_mode(LED_MODE_OFF); server.send(200, "​text/plain", "ok");});
  server.on("/save_eeprom",   [](){eeprom_write(); server.send(200, "​text/plain", "ok");});
  server.on("/toggle_meander",   [](){eeprom_toggleLedMatrixMeanderMode();led_control_set_led_matrix_meander_mode(eeprom_getLedMatrixMeanderMode()); server.send(200, "​text/plain", "ok");});
  server.on("/restart",   [](){ server.send(200, "​text/plain", "ok");ESP.restart();});
  

  server.onNotFound(handle_not_found);

  server.begin();

  stream_server.begin();
  stream_server.setNoDelay(true);
  
  TRACE_OUT();
}

void handle_requests() {
  server.handleClient();

  if (stream_server.hasClient()) {
    stream_client = stream_server.available();
  }

  int16_t x = 0;
  int16_t y = 0;
  while (stream_client.available()) {
    byte r = stream_client.read();
    byte g = stream_client.read();
    byte b = stream_client.read();

    led_matrix->draw_pixel(x, y, led_matrix->Color(r, g , b));
//    Serial.println("x=" + String(x) + "y=" + String(y) + "r=" + String(r) + "g=" + String(g) + "b=" + String(b));
    x++;
    if (x == led_control_matrix_width) {
      x = 0;
      y++;
    }

    if (y == led_control_matrix_height) {
      Serial.println("x=" + String(x) + "y=" + String(y) + "r=" + String(r) + "g=" + String(g) + "b=" + String(b));
      break;
    }
  }  
}

#include "main_page.h"
void handle_main_page() {
  TRACE_IN();

  server.send(200, "text/html", main_page);
  
  TRACE_OUT();
}

#include "scripts.h"
void handle_scripts() {
  TRACE_IN();
  
  server.send(200, "​application/javascript", scripts);

  TRACE_OUT();
}

String get_colors_of_all_leds() {
  TRACE_IN();
  
  String content = String();

  for (int x = 1; x <= led_control_matrix_width; x++) {
    for (int y = 1; y <= led_control_matrix_height; y++) {
      TRACE_VAR("x", String(x));
      TRACE_VAR("y", String(y));
      //uint32_t color = led_control_get_cached_color(x, y);
      TRACE_VAR("blah", "blah");
//      TRACE_VAR("color", String((int)color, HEX));
      //String color_as_string = color_to_string(color);
      //TRACE_VAR("color_as_string", color_as_string);
      //content = content + String(x) + ";" + String(y) + ";" + color_as_string + "\n";
    }
  }

  TRACE_OUT();
  
  return content;
}

void handle_get_data() {
  TRACE_IN();
  
  if (!server.hasArg("item"))
    return;

  String item = server.arg("item");

  if (item == "width") {
    server.send(200, "​text/plain", String(led_control_matrix_width));

    TRACE_VAR("width", String(led_control_matrix_width));
    TRACE_OUT();
      
    return;
  } else if (item == "height") {
    server.send(200, "​text/plain", String(led_control_matrix_height));
    
    TRACE_VAR("width", String(led_control_matrix_height));
    TRACE_OUT();
      
    return;
  } else if (item == "color") {
    if (server.hasArg("x") && server.hasArg("y")) {
      String x = server.arg("x");
      String y = server.arg("y");
      //String color = color_to_string(led_control_get_cached_color(x.toInt(), y.toInt()));
      //server.send(200, "​text/plain", x + ";" + y + ";" + color);
    
      TRACE_VAR("x", String(x));
      TRACE_VAR("x", String(y));
      //TRACE_VAR("color", color);
      TRACE_OUT();
      
      return;
    } else {
      String content = get_colors_of_all_leds();
      server.send(200, "​text/plain", content);
    
      TRACE_OUT();
      
      return;
    } 
  } else if (item == "brightness") {
      server.send(200, "​text/plain", String(eeprom_getLedMatrixBrightness()));
      Serial.println("brightness requested : " + String(eeprom_getLedMatrixBrightness()));
      return;
  }
  server.send(400, "text/plain", "Unrecognized arguments to get_data");

  TRACE_OUT();
}

void handle_set_data() {
  TRACE_IN();
  
  if (!server.hasArg("item"))
    return;

  String item = server.arg("item");

  if (item == "color") {
    if (server.hasArg("x") && server.hasArg("y") && server.hasArg("value")) {
      String xs = server.arg("x");
      String ys = server.arg("y");
      int x = xs.toInt();
      int y = ys.toInt();
      uint32_t color = string_to_color(server.arg("value"));
      //led_control_set_cached_color(x, y, color);
      //led_control_update();
      server.send(200, "​text/plain", "ok");
      return;
    }
  } else if (item == "brightness") {
    int brightness = server.arg("value").toInt();
    eeprom_setLedMatrixBrightness(brightness);
    Serial.println(" brightness web set " + String(brightness));
    server.send(200, "​text/plain", "ok");
    return;
  }
  
  server.send(400, "text/plain", "Unrecognized arguments to set_data");

  TRACE_OUT();
}

void handle_not_found() {
  TRACE_IN();
  
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

  TRACE_OUT();
}
