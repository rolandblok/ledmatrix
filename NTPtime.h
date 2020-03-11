#ifndef NTPtime
#define NTPtime

#include <Arduino.h>


time_t getNtpTime() ;

void NTPSetup() ;

String getStrTime() ;
String getStrDate() ;

#endif
