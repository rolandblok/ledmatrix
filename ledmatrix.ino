
#include "non_volatile_data.h"
#include "led_control.h"
#include "utils.h"
#include "wifi_aps.h"
#include "tracing.h"
#include "NTPtime.h"

#define TRACE_ALL false

// Define some function to setup different parts of the setup process

void setup_potmeter() ;
int  read_potmeter();

void setup_serial_communication() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial communication started, press ? for help");
}




// setup() function -- runs once at startup --------------------------------

void setup() {
  setup_serial_communication();

  //eeprom_clear();
  eeprom_init();
  eeprom_serial();
  
  tracing_set_output_on_serial(false);
  setup_wifi_aps();
  led_control_setup(eeprom_getLedMatrixWidth(), eeprom_getLedMatrixHeight());
  
  setup_potmeter();
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  
  handle_wifi() ;
  handle_leds();
  handle_serial();
}

void handle_serial() {
  if (TRACE_ALL) TRACE_IN();

  enum serial_input {NO_INPUT = 0, WIFI_SSID, WIFI_PWD, WIDTH, HEIGHT} ;
  static enum serial_input input_on = NO_INPUT;
  
  static int number_of_networks = 0;
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(10);
    static int wifi_selected_id = -1;

    if (command.equals("wifi")) {
      Serial.println("Scanning WiFi");
      number_of_networks = wifi_scan();
      for (int i = 0; i < number_of_networks; i++ ) {
        Serial.println("" + String(i) + " " + wifi_get_scanned_ssid(i) + " " + String(wifi_get_scanned_rssi(i)));
      }
      input_on = WIFI_SSID;
      Serial.println("SELECT SSID no");

    } else if ( input_on == WIFI_SSID) {
      wifi_selected_id = command.toInt();
      if (number_of_networks > wifi_selected_id) {
        Serial.print(" network selected " + String(wifi_selected_id));
        Serial.println(" " + wifi_get_scanned_ssid(wifi_selected_id) + " " + String(wifi_get_scanned_rssi(wifi_selected_id)));
        Serial.println("ENTER PASSWORD");
        input_on = WIFI_PWD;
      } else {
        Serial.println(" not available");
        wifi_add_scanned_ap(-1, "");
        input_on = NO_INPUT;
      }
    } else if (input_on == WIFI_PWD) {
      wifi_add_scanned_ap(wifi_selected_id, command);
      Serial.println("----------");
      Serial.println(" " + wifi_get_scanned_ssid(wifi_selected_id) + " - " + command);
      Serial.println("----------");
      input_on = NO_INPUT;
      
    } else if (input_on == WIDTH) {
      eeprom_setLedMatrixWidth(command.toInt());
      input_on = HEIGHT;
      Serial.println("INPUT HEIGHT");
    } else if (input_on == HEIGHT) {
      eeprom_setLedMatrixHeight(command.toInt());
      Serial.println("----------");
      Serial.println(" aspect (w:h) : " + String(eeprom_getLedMatrixWidth()) + " : " + String(eeprom_getLedMatrixHeight()));
      eeprom_write();
      led_control_set_led_matrix_size(eeprom_getLedMatrixWidth(), eeprom_getLedMatrixHeight());
      input_on = NO_INPUT;

    } else if (command.equals("wlist")) {
      Serial.println("----------");
      Serial.println("Stored SSID + pwds : " + String(eeprom_getNoWifiAps()));
      for (int i = 0; i < eeprom_getNoWifiAps(); i++) {
        WifiApEE wifi_ap = eeprom_getWifiAp(i);
        Serial.println(" " + String(i+1) + " : " + wifi_ap.ssid + " - " + wifi_ap.pwd);
      }
      Serial.println("----------");

    } else if (command.equals("wclear")) {
      Serial.println("clearing stored SSID + pwds");
      clear_wifi_apps();
      Serial.println("----------");
      
    } else if ((command.equals("IP")) || (command.equals("ip"))) {
      Serial.println(wifi_get_local_IP());
    } else if (command.equals("eepl")) {
      eeprom_serial();
    } else if (command.equals("eepw")) {
      eeprom_write();
    } else if (command.equals("eepc")) {
      eeprom_clear();
      eeprom_serial();
    } else if (command.equals("aspect")) {
      Serial.println("INPUT WIDTH");
      input_on = WIDTH;
    } else if (command.equals("time")) {
      Serial.println(" " + getStrTime() + " " + getStrDate());
    } else if (command.equals("restart")) {
      ESP.restart();
    } else if (command.equals("b_mode")) {
      int b_mode = (int)(eeprom_getLedMatrixBrightnessMode()) + 1;
      if (b_mode == EEPROM_BRIGHTNESS_MODES_COUNT) {
        b_mode = EEPROM_BRIGHTNESS_MODES_START;
      }
      eeprom_setLedMatrixBrightnessMode((EEPROM_BRIGHTNESS_MODES)b_mode);
      Serial.println("Brightness mode set: " + String(EEPROM_BRIGHTNESS_MODES_STR[eeprom_getLedMatrixBrightnessMode()]));
      Serial.println("----------");
    } else  {
      Serial.println("commands: ");
      Serial.println("  wifi   : scan available wifi and select");
      Serial.println("  wlist  : list stored ssid + pwd");
      Serial.println("  wclear : clear stored ssid + pwd");
      Serial.println("  ip     : get local IP");
      Serial.println("  eepl   : list eeprom stored data");
      Serial.println("  eepw   : write eeprom with current memory");
      Serial.println("  eepc   : clear eeprom");
      Serial.println("  aspect : set matrix aspect (width / height)");
      Serial.println("  time   : show time");
      Serial.println("  b_mode : change brightness mode, currently: " + String(EEPROM_BRIGHTNESS_MODES_STR[eeprom_getLedMatrixBrightnessMode()]));
      Serial.println("  restart: restart micro controller");
    }
  }

  if (TRACE_ALL) TRACE_OUT();
}

void handle_leds() {
  if (TRACE_ALL) TRACE_IN();

  static unsigned long last_update_ms = millis();
  static unsigned long FPS_update_ms = millis();
  static unsigned long FPS = 0;
  
  int current_time_ms = millis();
  
  if (current_time_ms - last_update_ms < 14) {
    if (TRACE_ALL) TRACE_OUT();
    
  } else {

    if (eeprom_getLedMatrixBrightnessMode() == EEPROM_BRIGHTNESS_MODES_POTMETER) {
      led_control_set_brightness(read_potmeter());
    } else {
      led_control_set_brightness(eeprom_getLedMatrixBrightness());
    }
    //  led_matrix->setBrightness(255);

    
    led_control_update(current_time_ms);
    last_update_ms = current_time_ms;

    FPS ++;
    if (current_time_ms - FPS_update_ms > 1000) {
      //Serial.println(" FPS : " + String(FPS));
      FPS = 0;
      FPS_update_ms = current_time_ms;
    }
    
    if (TRACE_ALL) TRACE_OUT();
  }
}

/**
 * enable the analog pin for potmeter
 */
void setup_potmeter() {  //Potentiometer for brightness
  pinMode(A0, INPUT); 
}  
/*
 * read the potmeter, return value between 0 - 255
 */
int read_potmeter() {
  int potentiometer = analogRead(A0);
  return(255*potentiometer/1024.0);
}
