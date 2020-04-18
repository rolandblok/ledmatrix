#ifndef WIFI_APS
#define WIFI_APS

#include <Arduino.h>

#include "non_volatile_data.h"


// https://github.com/arendst/Tasmota/issues/1943


//=====================
// setup&loop functions

void    setup_wifi_aps() ;
boolean handle_wifi() ;

int    wifi_scan();   // NOTE : disables wifi handling, always enable again.
String wifi_get_scanned_ssid(int i);
long   wifi_get_scanned_rssi(int i);
boolean wifi_add_scanned_ap(int i, String pwd);      // enables wifi handling, 

void    clear_wifi_apps();

#endif 
