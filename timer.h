#ifndef TIMER
#define TIMER
 
class Timer
{
  private:
      double _seconds;
      double _frequency;
 
  public:
      Timer(double frequency);

      int16_t time_in_period(double period);
      bool is_in_period(double period, double lower, double upper);
};
 
#endif
