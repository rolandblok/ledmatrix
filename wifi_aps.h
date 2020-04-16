#ifndef WIFI_APS
#define WIFI_APS

#include <Arduino.h>
#include <ESP8266WiFiMulti.h>

#include "non_volatile_data.h"

extern ESP8266WiFiMulti wifiMulti;

// https://github.com/arendst/Tasmota/issues/1943



//================
// setup functions

/**
 * read WIFI_aps from eeprom, and load to the wifi app
 */
void setup_wifi_aps() ;

/**
 * add an wifi ap, return true if succes/fits
 */
boolean wifi_ap_add_wifi_ap(WifiApEE new_wifi_ap) ;

void clear_wifi_apps();

#endif 
