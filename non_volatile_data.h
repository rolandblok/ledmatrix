#ifndef NON_VOLATILE_DATA_AA
#define NON_VOLATILE_DATA_AA

#include <Arduino.h>

//==========================
// Rolands versie hard coded

#define EEPC_WIFI_AP_MAX_APS      (5)
#define EEPC_WIFI_AP_SSID_MAX_LEN (33)
#define EEPC_WIFI_AP_PWD_MAX_LEN  (64)
typedef struct WifiApEE { 
  String ssid;
  String pwd;
} WifiApEE;

void setLedMatrixHeight(int h);
int  getLedMatrixHeight();
void setLedMatrixWidth(int w);
int  getLedMatrixWidth();

void   addWifiAps(WifiApEE wifi_ap);
int    getNoWifiAps();
WifiApEE getWifiApp(int id);
void   clearWifiAps();

boolean eeprom_init();
boolean eeprom_write();
boolean eeprom_clear();
void    eeprom_serial();

//==========================
// WJ versie, to be inplemented
/*
byte KEY_WIDTH = 1;
byte KEY_SSID = 2;
byte KEY_PASSWORD = 3;

void read_store();
void write_store();
void dump_store();
void add_string_to_store(byte key, String data);
void add_int_to_store(byte key, String data);
String get_string_from_store(byte key);
int get_int_from_store(byte key);
*/
#endif
