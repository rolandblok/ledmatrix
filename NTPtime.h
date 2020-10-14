#ifndef NTPtime
#define NTPtime

#include <Arduino.h>


time_t getNtpTime() ;

void NTPSetup() ;

int    getHour() ;
int    getMinute() ;
String getStrTime() ;
String getStrDate() ;

#endif
