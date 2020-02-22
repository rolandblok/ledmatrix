//#include "non_volatile_data.h"
//
//#include "tracing.h"
//
//#include <EEPROM.h>
//
//const byte FIRST_BYTE = 0xA2;
//const byte SECOND_BYTE = 0x05;
//const byte THIRD_BYTE = 0xB0;
//const byte FOURTH_BYTE = 0x64;
//
//int EEPROM_SIZE = 4096;
//
//byte KEY_WIDTH = 1;
//byte KEY_SSID = 2;
//byte KEY_PASSWORD = 3;
//
//struct store_entry {
//    byte key;
//    byte data_size;
//    byte *data;
//}
//
//byte nbr_store_entries = 0;
//struct store_entry *store_entries;
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
//  TRACE_VAR("first_byte", "0x"+String(first_bute, HEX));
//  byte second_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("second_byte", "0x"+String(first_bute, HEX));
//  byte third_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("third_byte", "0x"+String(first_bute, HEX));
//  byte fourth_byte = EEPROM.read(memory_index++);
//  TRACE_VAR("fourth_byte", "0x"+String(first_bute, HEX));
//
//  if ( (first_byte == FIRST_BYTE) && (second_byte == SECOND_BYTE) && (third_byte == THIRD_BYTE) && (fourth_byte == FOURTH_BYTE)) {
//    nbr_store_entries = 0;
//  }
//  else {
//    nbr_store_entries = EEPROM.read(memory_index++);
//    store_entries = (struct store_entry*) malloc(nbr_store_entries * sizeof(struct store_entry*));
//    for (int store_entry_index = 0; store_entry_index < nbr_store_entries; store_entry_index) {
//        store_entries[store_entry_index].key = EEPROM.read(memory_index++);
//        data_sistore_entries[store_entry_index].data_size = EEPROM.read(memory_index++);
//        data_sistore_entries[store_entry_index].data = read_bytes(memory_index, store_entries[store_entry_index].data_size);
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
