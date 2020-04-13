#ifndef NON_VOLATILE_DATA
#define NON_VOLATILE_DATA

#include <Arduino.h>

//==========================
// ROlands versie hard coded
#define WIFI_AP_MAX_APS      (5)
#define WIFI_AP_SSID_MAX_LEN (33)
#define WIFI_AP_PWD_MAX_LEN  (64)

typedef struct WifiApEE { 
    char ssid_buf[WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[WIFI_AP_SSID_MAX_LEN];
} ;

typedef struct EepromMem {
  byte   valid;
  int    led_matrix_width;
  int    led_matrix_height;
  byte   no_wifi_aps;
  WifiApEE wifi_aps[WIFI_AP_MAX_APS] ; 
  byte   checksum;
};

struct EepromMem eeprom_mem_glb = {};

boolean eeprom_init();
boolean eeprom_write();
boolean eeprom_clear();
void    eeprom_serial();

//==========================
// WJ versie, to be inplemented

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

#endif
