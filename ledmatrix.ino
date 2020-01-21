#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

//#define __WJ 


#ifdef __WJ
  #define LED_PIN D6
  #define BLINK_PIN D4
  #define LED_MATRIX_WIDTH 1
  #define LED_MATRIX_HEIGHT 4
  #define LED_MATRIX_SIZE (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#else
  #define LED_PIN   4    // : [NODEMCU:D2]  
  #define BLINK_PIN 5    // : [NODEMCU:D1]  
  #define LED_MATRIX_WIDTH 7
  #define LED_MATRIX_HEIGHT 7
  #define LED_MATRIX_SIZE (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#endif

Adafruit_NeoPixel strip(LED_MATRIX_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t led_colors[LED_MATRIX_SIZE] = {strip.Color(255,0,0), strip.Color(0,255,0), strip.Color(0,0,255), strip.Color(255,255,255)};

#include "utils.h"

// Define some function to setup different parts of the setup process

void setup_serial_communication() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial communication started");
}

// Try to connect to one of a couple of WiFi networks
void setup_wifi() {

  
  wifiMulti.addAP("Capibara", "waterzwijn");
  wifiMulti.addAP("fruitzender", "Knorknorknor1");

  // Keep checking the connection status until it is connected
  while (wifiMulti.run() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("SSID: " + WiFi.SSID() + "; IP address: " + WiFi.localIP().toString());
}

#include "web_handlers.h"
void setup_web_server() {
  Serial.print("Starting setup of web server...");

  server.on("/", handle_main_page);
  server.on("/scripts.js", handle_scripts);
  server.on("/get_data", handle_get_data);
  server.on("/set_data", handle_set_data);
  server.on("/dump", handle_dump_led_data);

  server.onNotFound(handle_not_found);

  server.begin();
  
  Serial.println(" Done");
}

void setup_led_matrix() {
  pinMode(BLINK_PIN, OUTPUT);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.clear();
  strip.show();            // Turn OFF all pixels ASAP
}

// setup() function -- runs once at startup --------------------------------

void setup() {
  setup_serial_communication();
  setup_wifi();
  setup_web_server();
  setup_led_matrix();
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  server.handleClient();
  handle_leds();
  handle_serial();
}

void handle_serial() {
  static int number_of_networks = 0;
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(10);
    Serial.println(" pressed : " + command);
  
    if (command.equals("w")) {
      Serial.print("Scanning WiFi");
    
      number_of_networks = WiFi.scanNetworks();
      Serial.println("networks");
      for (int i = 0; i < number_of_networks; i++ ) {
        Serial.println("" + String(i) + " " + WiFi.SSID(i) + " " + WiFi.RSSI(i));
      }
      Serial.println("----------");
    } else if (command.startsWith("c ")) {
      if (command.length() > 2) {
        long network_selected = command.substring(2).toInt();
        Serial.print(" network selected " + String(network_selected));
        if (number_of_networks > network_selected) {
          Serial.println(" " + WiFi.SSID(network_selected) + " " + WiFi.RSSI(network_selected));
          Serial.println("GIVE PASSWORD");
          
        } else {
          Serial.println(" not available");
        }
      }
    } else  {
      Serial.println("press");
      Serial.println("  w      : available wifi");
      Serial.println("  c <nr> : connect wifi network nr");
    }
  }
}

void handle_leds() {
  static unsigned long last_update = millis();

  int current = millis();
  if (current-last_update < 40)
    return;

  for (int x=1; x<=LED_MATRIX_WIDTH; x++) {
    for (int y=1; y<=LED_MATRIX_HEIGHT; y++) {
      int i = xy_to_index(x,y);
      strip.setPixelColor(i, led_colors[i]);
    }
  }
  strip.show();
  last_update = current;
}
