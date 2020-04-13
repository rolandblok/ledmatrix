#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

ESP8266WiFiMulti wifiMulti;

#include "led_control.h"
#include "utils.h"
#include "wifi_ap.h"
#include "web_handlers.h"
#include "tracing.h"
#include "NTPtime.h"
#include "non_volatile_data.h"

#define TRACE_ALL false

boolean wifi_connected       = false;
boolean wifi_set_connecting  = true;   // need to be able to stop for manual connecting.

// Define some function to setup different parts of the setup process

void setup_serial_communication() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial communication started, press ? for help");
}

// Try to connect to one of a couple of WiFi networks
void handle_wifi() {
  if (TRACE_ALL) TRACE_IN();

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
    NTPSetup();
    handle_requests();
  }

  if (TRACE_ALL) TRACE_OUT();
}

/** 
 *  Use this in debugging to reset your eeprom
 */
void EEPROM_reset() {
  EEPROM.begin(512);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
}

// setup() function -- runs once at startup --------------------------------

void setup() {
  //EEPROM_reset();
  setup_serial_communication();
  tracing_set_output_on_serial(false);
  setup_wifi_aps();
  led_control_setup();
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  handle_wifi();
  handle_leds();
  handle_serial();
}

void handle_serial() {
  if (TRACE_ALL) TRACE_IN();

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
        if (glb_no_wifi_aps < WIFI_AP_MAX_APS) {
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
    } else if (command.equals("eepl")) {
      eeprom_serial();
    } else if (command.equals("eepc")) {
      eeprom_clear();
    } else  {
      Serial.println("commands: ");
      Serial.println("  wifi   : scan available wifi and select");
      Serial.println("  list   : list stored ssid + pwd");
      Serial.println("  clear  : clear stored ssid + pwd");
      Serial.println("  eepl   : list eeprom stored data");
      Serial.println("  eepc   : clear eeprom");
    }
  }

  if (TRACE_ALL) TRACE_OUT();
}

void handle_leds() {
  if (TRACE_ALL) TRACE_IN();

  static unsigned long last_update = millis();
  int current = millis();
  
  if (current - last_update < 40) {
    if (TRACE_ALL) TRACE_OUT();
    return;
  }

  led_control_update();
  last_update = current;

  if (TRACE_ALL) TRACE_OUT();
}
