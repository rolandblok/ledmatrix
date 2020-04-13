
#include "tracing.h"

// https://github.com/arendst/Tasmota/issues/1943
#define WIFI_AP_MAX_APS      (5)
#define WIFI_AP_SSID_MAX_LEN (33)
#define WIFI_AP_PWD_MAX_LEN  (64)
#define WIFI_AP_EEPROM_SIZE  (1+WIFI_AP_MAX_APS*(WIFI_AP_SSID_MAX_LEN+WIFI_AP_PWD_MAX_LEN))

typedef struct WifiAp { 
  String ssid;
  String pwd;
};

//================
// globals

byte          glb_no_wifi_aps               = 0;
struct WifiAp glb_wifi_aps[WIFI_AP_MAX_APS] ; 

//================
// setup functions

/**
 * read WIFI_aps from eeprom, and load to the wifi app
 */
void setup_wifi_aps() {
  TRACE_IN();

  EEPROM.begin(WIFI_AP_EEPROM_SIZE);
  glb_no_wifi_aps = EEPROM.read(0);

  TRACE_VAR("glb_no_wifi_aps", String(glb_no_wifi_aps));

  int eep_adr = 1;
  for (int ap_nr = 0; ap_nr < glb_no_wifi_aps; ap_nr++) {
    TRACE_VAR("ap_nr", String(ap_nr));
    
    glb_wifi_aps[ap_nr].ssid = String();
    for (int ssid_char = 0; ssid_char < WIFI_AP_SSID_MAX_LEN; ssid_char++) {
      char c = EEPROM.read(eep_adr);
      glb_wifi_aps[ap_nr].ssid.concat(c);
      eep_adr ++ ;
    }
    TRACE_VAR("glb_wifi_aps[.].ssid", glb_wifi_aps[ap_nr].ssid);

    glb_wifi_aps[ap_nr].pwd = String();
    for (int pwd_char = 0; pwd_char < WIFI_AP_PWD_MAX_LEN; pwd_char++) {
      char c = EEPROM.read(eep_adr);
      glb_wifi_aps[ap_nr].pwd.concat(c);
      eep_adr ++ ;
    }
    TRACE_VAR("glb_wifi_aps[.].pwd", glb_wifi_aps[ap_nr].pwd);
    
    char ssid_buf[WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[WIFI_AP_SSID_MAX_LEN];
    glb_wifi_aps[ap_nr].ssid.toCharArray(ssid_buf, WIFI_AP_SSID_MAX_LEN);
    glb_wifi_aps[ap_nr].pwd.toCharArray(pwd_buf, WIFI_AP_PWD_MAX_LEN);
    wifiMulti.addAP(ssid_buf, pwd_buf);
  }

  EEPROM.end();
  
  TRACE_OUT();
}

/**
 * add an wifi ap, return true if succes/fits
 */
boolean wifi_ap_add_wifi_ap(struct WifiAp new_wifi_ap) {
  TRACE_IN();
  TRACE_VAR("wifi_ap_add_wifi_ap", new_wifi_ap.ssid);
  TRACE_VAR("wifi_ap_add_wifi_ap", new_wifi_ap.pwd);
  TRACE_VAR("glb_no_wifi_aps", String(glb_no_wifi_aps));

  if ( glb_no_wifi_aps == WIFI_AP_MAX_APS) {
    TRACE_OUT();
    return false;
  } else {
    // set to global
    glb_wifi_aps[glb_no_wifi_aps].ssid = String(new_wifi_ap.ssid);
    glb_wifi_aps[glb_no_wifi_aps].pwd  = String(new_wifi_ap.pwd);

    EEPROM.begin(WIFI_AP_EEPROM_SIZE);

    glb_no_wifi_aps ++;
    EEPROM.write(0, glb_no_wifi_aps);

    unsigned int wifi_ap_adr = 1;
    for (int ap_nr = 0; ap_nr < glb_no_wifi_aps; ap_nr ++) {
      for (unsigned int j = 0; j < WIFI_AP_SSID_MAX_LEN; j ++) {
        if (j < new_wifi_ap.ssid.length()) {
          EEPROM.write(wifi_ap_adr, glb_wifi_aps[ap_nr].ssid.charAt(j));
        } else {
          EEPROM.write(wifi_ap_adr, 0);
        }
        wifi_ap_adr ++;
      }
      for (unsigned int j = 0; j < WIFI_AP_PWD_MAX_LEN; j ++) {
        if (j < new_wifi_ap.ssid.length()) {
          EEPROM.write(wifi_ap_adr, glb_wifi_aps[ap_nr].pwd.charAt(j));
        } else {
          EEPROM.write(wifi_ap_adr, 0);
        }
        wifi_ap_adr ++;
      }
    }


    EEPROM.commit();
    EEPROM.end();

    char ssid_buf[WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[WIFI_AP_PWD_MAX_LEN];
    new_wifi_ap.ssid.toCharArray(ssid_buf, WIFI_AP_SSID_MAX_LEN);
    new_wifi_ap.pwd.toCharArray(pwd_buf, WIFI_AP_PWD_MAX_LEN);
    wifiMulti.addAP(ssid_buf, pwd_buf);
    
    
    return true;
    TRACE_OUT();
  }  
 
  TRACE_OUT();
}

/**
 * clear all aps and pwds
 */
void wifi_ap_clear_wifi_aps() {
  TRACE_IN();
  
  EEPROM.begin(WIFI_AP_EEPROM_SIZE);

  glb_no_wifi_aps = 0;
  EEPROM.write(0, 0);
  EEPROM.commit();
  EEPROM.end();

  TRACE_OUT();
}
