#include "NTPtime.h"
#include "utils.h"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// =====================
// globals
WiFiUDP       ntp_udp;                // A UDP instance to let us send and receive packets over UDP
IPAddress     ntp_time_server_IP;     // time.nist.gov NTP server address
const char*   ntp_server_name = "0.nl.pool.ntp.org";
unsigned int  ntp_port        = 2390; // local port to listen for UDP packets
#define       NTP_PACKET_SIZE 48      // NTP time stamp is in the first 48 bytes of the message
const int     ntp_time_zone = 1;      // Central European Time

time_t        ntp_last_successfull_sync = 0;  // timestamp of last successfull sync

volatile boolean    ntp_refresh_lock = false;

/**
 * setup the utp function for time syncing.
 */
void NTPSetup() {

  static bool ntp_setuped = false;
  
  if (!ntp_setuped) {
    ntp_udp.begin(ntp_port);
  
    // resolve the IP adress
    WiFi.hostByName(ntp_server_name, ntp_time_server_IP); 
    Serial.print("# NTP: server : " +String(ntp_server_name) +" IP: ");
    Serial.println(ntp_time_server_IP);
    
    // connect the NTP 
    setSyncProvider(getNtpTime);
    setSyncInterval((time_t)60UL);          //refresh every minute

    ntp_setuped = true;
  }
  return;
}


/**
 * Function to update Ntp time.
 */
time_t getNtpTime() {

  time_t  ntp_time      = 0UL;

  // prevent multiple parallel requestest to update (not understood, but happening)
  if (!ntp_refresh_lock) {
    ntp_refresh_lock = true;
    long    start_time    = millis();
    long    end_time      = start_time;
    boolean time_received = false;
    int     retry         = 0;
  
    while ((retry < 1) && (!time_received)) {
      Serial.println("# NTP: getNtpTime: sending NTP packet (try # " + String(retry+1) +" )");
      while (ntp_udp.parsePacket() > 0) ; // discard any previously received packets
    
      byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
    
      // send an NTP request to the time server at the given address
      // sendNTPpacket(ntp_time_server_IP); // send an NTP packet to a time server
      // set all bytes in the buffer to 0
      memset(packetBuffer, 0, NTP_PACKET_SIZE);
      // Initialize values needed to form NTP request
      // (see URL above for details on the packets)
      packetBuffer[0] = 0b11100011;   // LI, Version, Mode
      packetBuffer[1] = 0;     // Stratum, or type of clock
      packetBuffer[2] = 6;     // Polling Interval
      packetBuffer[3] = 0xEC;  // Peer Clock Precision
      // 8 bytes of zero for Root Delay & Root Dispersion
      packetBuffer[12]  = 49;
      packetBuffer[13]  = 0x4E;
      packetBuffer[14]  = 49;
      packetBuffer[15]  = 52;
    
      // all NTP fields have been given values, now
      // you can send a packet requesting a timestamp:
      ntp_udp.beginPacket(ntp_time_server_IP, 123); //NTP requests are to port 123
      ntp_udp.write(packetBuffer, NTP_PACKET_SIZE);
      ntp_udp.endPacket();
      
      // wait to see if a reply is available
      long udp_read_start_time = millis();
      long udp_read_end_time   = start_time;
      time_received       = false;
      while ((!time_received) && ((udp_read_end_time - udp_read_start_time) < 50L)) {
        int size = ntp_udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
          ntp_udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
          unsigned long secsSince1900;
          // convert four bytes starting at location 40 to a long integer
          secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
          ntp_time      = secsSince1900 - 2208988800UL + ntp_time_zone * SECS_PER_HOUR;
          time_received = true;
        }
        udp_read_end_time = millis();  
      }
      retry ++;
      end_time = millis();
    }
    
    // summarize to serial
    if (!time_received) {
      Serial.println("# NTP: No NTP Response.");
    } else {
      // calculate clock error
      time_t clock_d_sync  = 0;  //delta with last sync (secs)
      time_t clock_verloop = 0;  //delta in time in arduino (secs)
      if (ntp_last_successfull_sync != 0) {
        clock_d_sync  = ntp_time - ntp_last_successfull_sync;
        clock_verloop = now() - ntp_time;
      }
      ntp_last_successfull_sync = ntp_time;
  
      // debug output:
      Serial.print("# NTP: getNtpTime: time received in " + String(end_time - start_time) + " ms");
      Serial.println(", using " + String(retry) + " tries.");
      Serial.print("# NTP: getNtpTime: time since last sync (s) : " + String(clock_d_sync));
      Serial.println(" ; verloop  (s) : " + String(clock_verloop));
    }
    
    ntp_refresh_lock = false;
  } else {
    Serial.println("# NTP: getNtpTime: refresh requested while locked (alraedy syncing). Will not execute.");
  }
  
  return ntp_time;
}

String getStrTime() {
  return (trail10print(hour(),true) + ":" + trail10print(minute(),true) + ":" + trail10print(second(),true) );
}
String getStrDate() {
    return (String(year()) + "-" + trail10print(month(),true) + "-" + trail10print(day(),true) );
}
