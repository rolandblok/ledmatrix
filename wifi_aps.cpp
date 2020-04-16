


#include "wifi_aps.h"

#include "tracing.h"
#include "non_volatile_data.h"


ESP8266WiFiMulti wifiMulti;


//================
// setup functions

/**
 * read WIFI_aps from eeprom, and load to the wifi app
 */
void setup_wifi_aps() {
  TRACE_IN();
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

/**
 * add an wifi ap, return true if succes/fits
 */
boolean wifi_ap_add_wifi_ap(WifiApEE new_wifi_ap) {
  TRACE_IN();
  TRACE_VAR("wifi_ap_add_wifi_ap", new_wifi_ap.ssid);
  TRACE_VAR("wifi_ap_add_wifi_ap", new_wifi_ap.pwd);

  eeprom_addWifiAp(new_wifi_ap);

  char ssid_buf[EEPC_WIFI_AP_SSID_MAX_LEN];
  char pwd_buf[EEPC_WIFI_AP_PWD_MAX_LEN];
  new_wifi_ap.ssid.toCharArray(ssid_buf, EEPC_WIFI_AP_SSID_MAX_LEN);
  new_wifi_ap.pwd.toCharArray(pwd_buf, EEPC_WIFI_AP_PWD_MAX_LEN);
  wifiMulti.addAP(ssid_buf, pwd_buf);

  eeprom_write();
  
  TRACE_OUT();
  return true;
}


void clear_wifi_apps() {
  eeprom_clearWifiAps();
}
