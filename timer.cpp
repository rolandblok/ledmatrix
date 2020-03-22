
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

int16_t Timer::get_location_back_and_forth(int16_t matrix_width, int16_t image_width, double period) {
  int16_t amplitude = (matrix_width - image_width)/2;
  return amplitude - amplitude*cos(1.0*_frequency*_seconds/period/2/PI);
}
