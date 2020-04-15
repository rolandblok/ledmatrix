#ifndef NON_VOLATILE_DATA_AA
#define NON_VOLATILE_DATA_AA

#include <Arduino.h>

//==========================
// Rolands versie hard coded
#define EEPC_WIFI_AP_MAX_APS      (5)
#define EEPC_WIFI_AP_SSID_MAX_LEN (33)
#define EEPC_WIFI_AP_PWD_MAX_LEN  (64)

typedef struct WifiApEE { 
    char ssid_buf[EEPC_WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[EEPC_WIFI_AP_SSID_MAX_LEN];
} ;

struct EepromMemo_struct {
  byte   valid;
  int    led_matrix_width;
  int    led_matrix_height;
  byte   no_wifi_aps;
  WifiApEE wifi_aps[EEPC_WIFI_AP_MAX_APS] ; 
  byte   checksum;
};

typedef struct EepromMemo_struct EepromMemo;

extern EepromMemo eeprom_memo_glb;

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
