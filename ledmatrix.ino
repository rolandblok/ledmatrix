#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>


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

#include "utils.h"
#include "wifi_ap.h"

Adafruit_NeoPixel strip(LED_MATRIX_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t led_colors[LED_MATRIX_SIZE] = {strip.Color(255,0,0), strip.Color(0,255,0), strip.Color(0,0,255), strip.Color(255,255,255)};

boolean wifi_connected       = false;
boolean wifi_set_connecting  = true;   // need to be able to stop for manual connecting.

// Define some function to setup different parts of the setup process


void setup_serial_communication() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial communication started, press ? for help");
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

// Try to connect to one of a couple of WiFi networks
void handle_wifi() {

  /*
  wifiMulti.addAP("Capibara", "xyz");
  wifiMulti.addAP("fruitzender", "zyx");

  // Keep checking the connection status until it is connected
  while (wifiMulti.run() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  */
  
  //WiFi.status() == WL_connected;
  if (!wifi_connected) {
    static unsigned long last_update_ms = millis();
    unsigned long now_ms = millis();
    if (((now_ms - last_update_ms) > 3000) && (wifi_set_connecting)) {
      last_update_ms = now_ms;

      if (wifiMulti.run() == WL_CONNECTED ) { 
        wifi_connected = true;
        Serial.println("SSID: " + WiFi.SSID() + "; IP address: " + WiFi.localIP().toString());
        setup_web_server();
      } else {
        Serial.println(" connection failed");
      }
      
    }
  } else {
      server.handleClient();
  }
  
}

// setup() function -- runs once at startup --------------------------------

void setup() {
  setup_serial_communication();
  setup_wifi_aps();
  setup_led_matrix();

}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  handle_wifi();

  handle_leds();
  handle_serial();
}

void handle_serial() {
  static int number_of_networks = 0;
  static boolean wifi_pwd_input_on  = false;
  static boolean wifi_ssid_input_on = false;
  static struct WifiAp new_wifi_ap ;
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(10);

    if ( wifi_ssid_input_on ) {
      wifi_ssid_input_on = false;
      long network_selected = command.toInt();
      if (number_of_networks > network_selected) {
        if (glb_no_wifi_aps < WIFI_AP_MAX_APS){
          Serial.print(" network selected " + String(network_selected));
          Serial.println(" " + WiFi.SSID(network_selected) + " " + WiFi.RSSI(network_selected));
          new_wifi_ap.ssid = "" + WiFi.SSID(network_selected);
          Serial.println("ENTER PASSWORD");
          wifi_pwd_input_on  = true;
        } else {
          Serial.println("max number of wifi app surpased [" + String(WIFI_AP_MAX_APS) + "]");
          wifi_set_connecting = true;
        }
      } else {
        Serial.println(" not available");
        wifi_set_connecting = true;
      }
      
    } else if (wifi_pwd_input_on) {
      new_wifi_ap.pwd = "" + command;
      wifi_ap_add_wifi_ap(new_wifi_ap);
      wifi_pwd_input_on   = false;
      wifi_set_connecting = true;
      Serial.println("----------");
      Serial.println(" " + new_wifi_ap.ssid + " - " + new_wifi_ap.pwd);
      
    } else if (command.equals("wifi")) {
      wifi_set_connecting = false; // disable connecting, otherwize it will overwrite scanned wifis
      Serial.println("Scanning WiFi");
      delay(1000);
      number_of_networks = WiFi.scanNetworks();
      for (int i = 0; i < number_of_networks; i++ ) {
        Serial.println("" + String(i) + " " + WiFi.SSID(i) + " " + WiFi.RSSI(i));
      }
      wifi_ssid_input_on = true;
      Serial.println("SELECT SSID no");
      
    } else if (command.equals("list")) {
      Serial.println("Stored SSID + pwds : " + String(glb_no_wifi_aps));
      for (int i = 0; i < glb_no_wifi_aps; i++) {
         Serial.println(" " + String(i) + " : " + glb_wifi_aps[i].ssid + " - " + glb_wifi_aps[i].pwd);
      }
      Serial.println("----------");
      
    } else if (command.equals("clear")) {
      Serial.println("clearing stored SSID + pwds");
      wifi_ap_clear_wifi_aps();
      Serial.println("----------");
      
    } else if (command.equals("eepc")) {
        EEPROM.begin(512);
        Serial.println("----------");
        for (int i = 0; i < 512 ; i++ ) {
          EEPROM.write(i, 0);
        }
        Serial.println("----------");
        EEPROM.end();
        
    } else if (command.equals("eep")) {
        EEPROM.begin(512);
        Serial.println("----------");
        for (int i = 0; i < 100 ; i++ ) {
          Serial.println("" + String(i) + " " +String(EEPROM.read(i)));
        }
        Serial.println("");
        Serial.println("----------");
        EEPROM.end();
        
    } else  {
      Serial.println("commands: ");
      Serial.println("  wifi   : scan available wifi and select");
      Serial.println("  list   : list stored ssid + pwd");
      Serial.println("  clear  : clear stored ssid + pwd");
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
