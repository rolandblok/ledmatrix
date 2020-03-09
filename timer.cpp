
#include <Arduino.h>
#include "timer.h"

Timer::Timer(double frequency) {
  unsigned long milliseconds = millis();
  _seconds = milliseconds/1000.0;
  _frequency = frequency;
}

int16_t Timer::time_in_period(double period) {
  return (int16_t) fmod(_frequency*_seconds, period);
}

bool Timer::is_in_period(double period, double lower, double upper) {
  double current = fmod(_frequency*_seconds, period);
  return (current >= lower) && (current < upper);
}
