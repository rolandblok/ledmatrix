


#include "wifi_aps.h"

#include <ESP8266WiFiMulti.h>

#include "web_handlers.h"
#include "NTPtime.h"
#include "tracing.h"
#include "non_volatile_data.h"


ESP8266WiFiMulti wifiMulti;
boolean wifi_connected       = false;
boolean wifi_set_connecting  = true;   // need to be able to stop for manual connecting.



//================
// setup functions

/**
 * read WIFI_aps from eeprom, and load to the wifi app
 */
void setup_wifi_aps() {
  TRACE_IN();
//  WiFi.disconnect(true); // remove wifi credentials from esp
  int no_wifi_apps = eeprom_getNoWifiAps();
  
  for (int ap_nr = 0; ap_nr < no_wifi_apps; ap_nr++) {
    TRACE_VAR("ap_nr", String(ap_nr));
    
    WifiApEE wifi_app = eeprom_getWifiAp(ap_nr);
    
    char ssid_buf[EEPC_WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[EEPC_WIFI_AP_PWD_MAX_LEN];
    wifi_app.ssid.toCharArray(ssid_buf, EEPC_WIFI_AP_SSID_MAX_LEN);
    wifi_app.pwd.toCharArray(pwd_buf, EEPC_WIFI_AP_PWD_MAX_LEN);
    wifiMulti.addAP(ssid_buf, pwd_buf);

  }
  TRACE_OUT();
  return;
}


//================
// loop  functions

// Try to connect to one of a couple of WiFi networks
boolean handle_wifi() {


  //WiFi.status() == WL_connected;
  if (!wifi_connected) {
    static unsigned long last_update_ms = millis();
    unsigned long now_ms = millis();
    if (((now_ms - last_update_ms) > 3000) && (wifi_set_connecting)) {
      last_update_ms = now_ms;
      WiFi.hostname("roland");
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
  
}

String wifi_get_local_IP() {
  return WiFi.localIP().toString();
}

/**
 * always let this follow with wifi_ap_add_wifi_ap, to enable wifi connecting again.
 */
int wifi_scan() {
  wifi_set_connecting = false; // disable connecting, otherwize it will overwrite scanned wifis
  delay(1000);
  return WiFi.scanNetworks();
}

String wifi_get_scanned_ssid(int i) {
  return WiFi.SSID(i);
}
long wifi_get_scanned_rssi(int i) {
  return WiFi.RSSI(i);
}

/**
 * add an wifi ap, return true if succes/fits
 *    if with NULL argument : just enables wifi scan again.
 */
boolean wifi_add_scanned_ap(int i, String pwd_str) {
  TRACE_IN();
  if (i >= 0) {


    eeprom_addWifiAp(WiFi.SSID(i), pwd_str);

    char ssid_char[EEPC_WIFI_AP_SSID_MAX_LEN];
    WiFi.SSID(i).toCharArray(ssid_char, EEPC_WIFI_AP_SSID_MAX_LEN);
    char pwd_char[EEPC_WIFI_AP_PWD_MAX_LEN];
    pwd_str.toCharArray(pwd_char, EEPC_WIFI_AP_PWD_MAX_LEN);

    wifiMulti.addAP(ssid_char, pwd_char);
   
    eeprom_write();
  }
  
  wifi_set_connecting = true;
  
  TRACE_OUT();
  return true;
}


void clear_wifi_apps() {
  eeprom_clearWifiAps();
}
