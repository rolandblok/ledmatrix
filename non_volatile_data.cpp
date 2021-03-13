#include "non_volatile_data.h"

#include "tracing.h"

#include <Arduino.h>
#include <EEPROM.h>

//=================================
// ROLAND IMPLEMENTATION hard coded
//=================================


#define EEPC_WIFI_AP_SSID_MAX_LEN (33)
#define EEPC_WIFI_AP_PWD_MAX_LEN  (64)

typedef struct WifiApChar { 
    char ssid_buf[EEPC_WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[EEPC_WIFI_AP_PWD_MAX_LEN];
} WifiApChar;

typedef struct EepromMem_struct {
  byte    valid;
  int     led_matrix_width;
  int     led_matrix_height;
  byte    no_wifi_aps;
  WifiApChar wifi_aps[EEPC_WIFI_AP_MAX_APS] ; 
  EEPROM_BRIGHTNESS_MODES brightness_mode;
  int     brightness;
  EEPROM_MATRIX_MODES matrix_mode;
  boolean meander_mode;
  byte    checksum;
} EepromMem;


//extern EepromMem eeprom_mem_glb;
EepromMem eeprom_mem_glb;

// ===============
// getters setters
// ===============
void eeprom_setLedMatrixHeight(int h) {
  eeprom_mem_glb.led_matrix_height = h;
  eeprom_write();
}
int  eeprom_getLedMatrixHeight() {
  return eeprom_mem_glb.led_matrix_height;
}
void eeprom_setLedMatrixWidth(int w) {
  eeprom_mem_glb.led_matrix_width = w;
  eeprom_write();
}
int  eeprom_getLedMatrixWidth() {
  return eeprom_mem_glb.led_matrix_width;
}

int    eeprom_getNoWifiAps() {
  return eeprom_mem_glb.no_wifi_aps;
}

void   eeprom_addWifiAp(String ssid, String pwd) {
  if ( eeprom_mem_glb.no_wifi_aps == 5 ) {
    eeprom_mem_glb.no_wifi_aps = 4; //we overwrite the last one :-)
  }
  ssid.toCharArray(eeprom_mem_glb.wifi_aps[eeprom_mem_glb.no_wifi_aps].ssid_buf, EEPC_WIFI_AP_SSID_MAX_LEN);
  pwd.toCharArray(eeprom_mem_glb.wifi_aps[eeprom_mem_glb.no_wifi_aps].pwd_buf, EEPC_WIFI_AP_PWD_MAX_LEN);
  
  eeprom_mem_glb.no_wifi_aps++;
  eeprom_write();
}

WifiApEE eeprom_getWifiAp(int id) {
  WifiApEE wifi_app = {};
  if ( (id >= 0) && (id < eeprom_mem_glb.no_wifi_aps)) {
    wifi_app.ssid = String(eeprom_mem_glb.wifi_aps[id].ssid_buf);
    wifi_app.pwd  = String(eeprom_mem_glb.wifi_aps[id].pwd_buf);
  }
  return wifi_app;
}

void eeprom_clearWifiAps() {
  eeprom_mem_glb.no_wifi_aps = 0;
  eeprom_write();
}

void eeprom_setLedMatrixBrightnessMode(EEPROM_BRIGHTNESS_MODES b_mode) {
  eeprom_mem_glb.brightness_mode = b_mode;
  eeprom_write();
}

EEPROM_BRIGHTNESS_MODES eeprom_getLedMatrixBrightnessMode() {
  return eeprom_mem_glb.brightness_mode;
}

void eeprom_setLedMatrixBrightness(int b) {
  eeprom_mem_glb.brightness = b;
  //eeprom_write();
}

int  eeprom_getLedMatrixBrightness() {
  return eeprom_mem_glb.brightness;
}

void eeprom_setLedMatrixMode(EEPROM_MATRIX_MODES m_mode) {
  eeprom_mem_glb.matrix_mode = m_mode;
  eeprom_write();
}

EEPROM_MATRIX_MODES eeprom_getLedMatrixMode() {
  return eeprom_mem_glb.matrix_mode;
}

void eeprom_toggleLedMatrixMeanderMode() {
  eeprom_mem_glb.meander_mode = !eeprom_mem_glb.meander_mode;
  eeprom_write();
}
boolean eeprom_getLedMatrixMeanderMode() {
  return eeprom_mem_glb.meander_mode;
}


byte checksum(EepromMem eeprom_memo_arg) {
  return eeprom_memo_arg.led_matrix_width + eeprom_memo_arg.led_matrix_height + eeprom_memo_arg.no_wifi_aps;
}


/** 
 *  Use this in debugging to reset your eeprom
 */
boolean eeprom_clear() {
  EepromMem eeprom_mem_tmp = {};
  eeprom_mem_glb = eeprom_mem_tmp;
  eeprom_write();
  
//  EEPROM.begin(sizeof(eeprom_mem_glb));
//  for (int i = 0; i < sizeof(eeprom_mem_glb); i++) {
//    EEPROM.write(i, 0);
//  }
//  EEPROM.commit();
//  EEPROM.end();

  return true;
}

boolean eeprom_init() {
  EepromMem eeprom_mem_tmp = {};
  EEPROM.begin(sizeof(EepromMem));
  EEPROM.get(0, eeprom_mem_tmp);
  EEPROM.end();
  if (eeprom_mem_tmp.valid == 1) {
    if (eeprom_mem_tmp.checksum == checksum(eeprom_mem_tmp)) {
      eeprom_mem_glb = eeprom_mem_tmp;
    } else {
      Serial.println("eeprom checksum invalid");
      return false;
    }
  } else {
    Serial.println("eeprom read invalid");
    return false;
  }

  return true;
}

boolean eeprom_write() {
  eeprom_mem_glb.valid = 1;
  eeprom_mem_glb.checksum = checksum(eeprom_mem_glb);
  EEPROM.begin(sizeof(eeprom_mem_glb));
  EEPROM.put(0, eeprom_mem_glb);
  EEPROM.commit();
  EEPROM.end();

  return true;
}

void eeprom_serial() {
  Serial.println("----------");
  Serial.println("led_matrix_width  : " + String(eeprom_mem_glb.led_matrix_width) );
  Serial.println("led_matrix_height : " + String(eeprom_mem_glb.led_matrix_height));
  Serial.println("n.o. wifi aps : " + String(eeprom_mem_glb.no_wifi_aps));
  for (int i = 0; i < eeprom_mem_glb.no_wifi_aps; i ++) {
    Serial.println(" " + String(i+1) + " " +String(eeprom_mem_glb.wifi_aps[i].ssid_buf));
    Serial.println("     " + String(eeprom_mem_glb.wifi_aps[i].pwd_buf));
  }
  Serial.println("  brightness mode : " + String(EEPROM_BRIGHTNESS_MODES_STR[eeprom_getLedMatrixBrightnessMode()]));
  Serial.println("  matrix mode : " + String(EEPROM_MATRIX_MODES_STR[eeprom_getLedMatrixMode()]));
  Serial.println("  meander mode : " + String(eeprom_getLedMatrixMeanderMode()?"TRUE":"FALSE"));
  Serial.println("----------");
}


//================================
// WJ IMPLEMENTATION NOT FINISHED;
//================================
//const byte FIRST_BYTE = 0xA2;
//const byte SECOND_BYTE = 0x05;
//const byte THIRD_BYTE = 0xB0;
//const byte FOURTH_BYTE = 0x64;
//
//int EEPROM_SIZE = 4096;
//
//struct store_entry {
//    byte key;
//    byte data_size;
//    byte *data;
//};
//
//byte nbr_store_entries = 0;
//struct store_entry *store_entries = NULL;
//
//byte* read_bytes(int memory_index, int nbr_bytes) {
//  TRACE_IN();
//
//  byte *data = (byte*) malloc(nbr_bytes);
//  
//  for (int data_index = 0; data_index < nbr_bytes; data_index) {
//      data[data_index] = EEPROM.read(memory_index++);
//  }
//
//  TRACE_OUT();
//
//  return data;
//}
//
//byte* write_bytes(int memory_index, byte nbr_bytes, byte *data)
//{
//  TRACE_IN();
//
//  for (int memory_offset = 0; memory_offset < nbr_bytes; memory_offset++) {
//      EEPROM.write(memory_index+memory_offset, data[memory_offset]);
//  }
//
//  TRACE_OUT();
//}
//
//void read_store() {
//  TRACE_IN();
//    
//  EEPROM.begin(EEPROM_SIZE);
//  
//  int memory_index = 0;
//  byte first_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("first_byte", "0x"+String(first_byte, HEX));
//  byte second_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("second_byte", "0x"+String(second_byte, HEX));
//  byte third_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("third_byte", "0x"+String(third_byte, HEX));
//  byte fourth_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("fourth_byte", "0x"+String(fourth_byte, HEX));
//
//  if ( (first_byte == FIRST_BYTE) && (second_byte == SECOND_BYTE) && (third_byte == THIRD_BYTE) && (fourth_byte == FOURTH_BYTE)) {
//    nbr_store_entries = 0;
//  }
//  else {
//    nbr_store_entries = EEPROM.read(memory_index++);
//    store_entries = (struct store_entry*) malloc(nbr_store_entries * sizeof(struct store_entry*));
//    for (int store_entry_index = 0; store_entry_index < nbr_store_entries; store_entry_index) {
//        store_entries[store_entry_index].key = EEPROM.read(memory_index++);
//        store_entries[store_entry_index].data_size = EEPROM.read(memory_index++);
//        store_entries[store_entry_index].data = read_bytes(memory_index, store_entries[store_entry_index].data_size);
//        memory_index += store_entries[store_entry_index].data_size;
//    }
//  }
//  
//  EEPROM.end();
//
//  TRACE_OUT();
//}
//
//void write_store() {
//  TRACE_IN();
//    
//  EEPROM.begin(EEPROM_SIZE);
//
//  int memory_index = 0;
//  EEPROM.write(memory_index++, FIRST_BYTE);
//  EEPROM.write(memory_index++, SECOND_BYTE);
//  EEPROM.write(memory_index++, THIRD_BYTE);
//  EEPROM.write(memory_index++, FOURTH_BYTE);
//  
//  EEPROM.write(memory_index++, nbr_store_entries);
//  for (int store_entry_index = 0; store_entry_index < nbr_store_entries; store_entry_index) {
//      EEPROM.write(memory_index++, store_entries[store_entry_index].key);
//      EEPROM.write(memory_index++, store_entries[store_entry_index].data_size);
//      write_bytes(memory_index, store_entries[store_entry_index].data_size, store_entries[store_entry_index].data);
//      memory_index += store_entries[store_entry_index].data_size;
//  }
//  EEPROM.end();
//
//  TRACE_OUT();
//}
//
//void add_entry_to_store(byte key, byte data_size, byte *data) {
//  TRACE_IN();
//  
//  store_entries = (struct store_entry*) realloc(store_entries, (nbr_store_entries + 1)  * sizeof(struct store_entry*));
//
//  store_entries[nbr_store_entries].key = key;
//  store_entries[nbr_store_entries].data_size = data_size;
//  store_entries[nbr_store_entries].data = data;
//
//  nbr_store_entries++;
//
//  TRACE_OUT();
//}
//
//void add_string_to_store(byte key, String data) {
//  TRACE_IN();
//  
//  char *char_prt = data.toCharArray();
//  add_entry(key, data.length, char_ptr);
//
//  TRACE_OUT();
//}
//
//void add_int_to_store(byte key, int data) {
//  TRACE_IN();
//  
//  add_entry(key, sizeof(int), &data);
//
//  TRACE_OUT();
//}
//
//
//
//int get_int_from_store(byte key) {
//
//}
