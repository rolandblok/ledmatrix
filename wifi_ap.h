
// https://github.com/arendst/Tasmota/issues/1943
#define WIFI_AP_MAX_APS      (5)
#define WIFI_AP_SSID_MAX_LEN (33)
#define WIFI_AP_PWD_MAX_LEN  (64)
#define WIFI_AP_EEPROM_SIZE  (1+WIFI_AP_MAX_APS*(WIFI_AP_SSID_MAX_LEN+WIFI_AP_PWD_MAX_LEN))

struct WifiAp { 
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
  Serial.println(" setup_wifi_aps " );
  EEPROM.begin(WIFI_AP_EEPROM_SIZE);
  glb_no_wifi_aps = EEPROM.read(0);

  int eep_adr = 1;
  for (int ap_nr = 0; ap_nr < glb_no_wifi_aps; ap_nr++) {
    glb_wifi_aps[ap_nr].ssid = String();
    for (int ssid_char = 0; ssid_char < WIFI_AP_SSID_MAX_LEN; ssid_char++) {
      char c = EEPROM.read(eep_adr);
      glb_wifi_aps[ap_nr].ssid.concat(c);
      eep_adr ++ ;
    }

    glb_wifi_aps[ap_nr].pwd = String();
    for (int pwd_char = 0; pwd_char < WIFI_AP_PWD_MAX_LEN; pwd_char++) {
      char c = EEPROM.read(eep_adr);
      glb_wifi_aps[ap_nr].pwd.concat(c);
      eep_adr ++ ;
    }
    char ssid_buf[WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[WIFI_AP_SSID_MAX_LEN];
    glb_wifi_aps[ap_nr].ssid.toCharArray(ssid_buf, WIFI_AP_SSID_MAX_LEN);
    glb_wifi_aps[ap_nr].pwd.toCharArray(pwd_buf, WIFI_AP_PWD_MAX_LEN);
    wifiMulti.addAP(ssid_buf, pwd_buf);
  }

  EEPROM.end();
}

/**
 * add an wifi ap, return true if succes/fits
 */
boolean wifi_ap_add_wifi_ap(struct WifiAp new_wifi_ap) {
  
  if ( glb_no_wifi_aps == WIFI_AP_MAX_APS) {
    return false;
  } else {
    // set to global
    glb_wifi_aps[glb_no_wifi_aps].ssid = String(new_wifi_ap.ssid);
    glb_wifi_aps[glb_no_wifi_aps].pwd  = String(new_wifi_ap.pwd);

    EEPROM.begin(WIFI_AP_EEPROM_SIZE);
    for (int ap_nr = 0; ap_nr < glb_no_wifi_aps; ap_nr ++) {
      unsigned int wifi_ap_adr = 1 + glb_no_wifi_aps*(WIFI_AP_SSID_MAX_LEN + WIFI_AP_PWD_MAX_LEN);
      for (unsigned int j = 0; j < WIFI_AP_SSID_MAX_LEN; j ++) {
        if (j < new_wifi_ap.ssid.length()) {
          EEPROM.write(wifi_ap_adr + j, new_wifi_ap.ssid.charAt(j));
        } else {
          EEPROM.write(wifi_ap_adr + j, 0);
        }
      }
      wifi_ap_adr += WIFI_AP_SSID_MAX_LEN;
      for (unsigned int j = 0; j < WIFI_AP_PWD_MAX_LEN; j ++) {
        if (j < new_wifi_ap.ssid.length()) {
          EEPROM.write(wifi_ap_adr + j, new_wifi_ap.pwd.charAt(j));

        } else {
          EEPROM.write(wifi_ap_adr + j, 0);
        }
      }
    }

    glb_no_wifi_aps ++;
    EEPROM.write(0, glb_no_wifi_aps);

    EEPROM.commit();
    EEPROM.end();

    char ssid_buf[WIFI_AP_SSID_MAX_LEN];
    char pwd_buf[WIFI_AP_SSID_MAX_LEN];
    new_wifi_ap.ssid.toCharArray(ssid_buf, WIFI_AP_SSID_MAX_LEN);
    new_wifi_ap.pwd.toCharArray(pwd_buf, WIFI_AP_PWD_MAX_LEN);
    wifiMulti.addAP(ssid_buf, pwd_buf);
    
    
    return true;
  }  
 
}

/**
 * clear all aps and pwds
 */
void wifi_ap_clear_wifi_aps() {
  EEPROM.begin(WIFI_AP_EEPROM_SIZE);

  glb_no_wifi_aps = 0;
  EEPROM.write(0, 0);
  EEPROM.commit();
  EEPROM.end();

}
