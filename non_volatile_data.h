#ifndef NON_VOLATILE_DATA_AA
#define NON_VOLATILE_DATA_AA

#include <Arduino.h>

//==========================
// Rolands versie hard coded
enum EEPROM_BRIGHTNESS_MODES {
  EEPROM_BRIGHTNESS_MODES_START = 0,
  EEPROM_BRIGHTNESS_MODES_POTMETER = 0,
  EEPROM_BRIGHTNESS_MODES_HTTP,
  EEPROM_BRIGHTNESS_MODES_COUNT
};
static const char* EEPROM_BRIGHTNESS_MODES_STR[EEPROM_BRIGHTNESS_MODES_COUNT] = 
{
  [EEPROM_BRIGHTNESS_MODES_POTMETER]  = "potmeter mode",
  [EEPROM_BRIGHTNESS_MODES_HTTP]   = "http mode"
};

enum EEPROM_MATRIX_MODES {
  EEPROM_MATRIX_MODES_START = 0,
  EEPROM_MATRIX_MODES_MATRIX = 0,
  EEPROM_MATRIX_MODES_DIGIT,
  EEPROM_MATRIX_MODES_COUNT
};
static const char* EEPROM_MATRIX_MODES_STR[EEPROM_MATRIX_MODES_COUNT] = 
{
  [EEPROM_MATRIX_MODES_MATRIX]  = "matrix mode",
  [EEPROM_MATRIX_MODES_DIGIT]   = "digit mode"
};



#define EEPC_WIFI_AP_MAX_APS      (5)
#define EEPC_WIFI_AP_SSID_MAX_LEN (33)
#define EEPC_WIFI_AP_PWD_MAX_LEN  (64)
typedef struct WifiApEE { 
  String ssid;
  String pwd;
} WifiApEE;

void eeprom_setLedMatrixHeight(int h);
int  eeprom_getLedMatrixHeight();
void eeprom_setLedMatrixWidth(int w);
int  eeprom_getLedMatrixWidth();
void eeprom_setLedMatrixBrightnessMode(EEPROM_BRIGHTNESS_MODES b_more);
EEPROM_BRIGHTNESS_MODES eeprom_getLedMatrixBrightnessMode();

void eeprom_setLedMatrixBrightness(int b);  // value 0 .. 255
int  eeprom_getLedMatrixBrightness();

void eeprom_setLedMatrixMode(EEPROM_MATRIX_MODES m_mode);
EEPROM_MATRIX_MODES eeprom_getLedMatrixMode();

void eeprom_toggleLedMatrixMeanderMode();
boolean eeprom_getLedMatrixMeanderMode();

void   eeprom_addWifiAp(String ssid, String pwd);
int    eeprom_getNoWifiAps();
WifiApEE eeprom_getWifiAp(int id);
void   eeprom_clearWifiAps();

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
