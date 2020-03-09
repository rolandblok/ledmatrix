#ifndef COLORS
#define COLORS
 
class Colors
{
  private:
      double _brightness;
 
  public:
      Colors(double brightness);

      int16_t get_matrix_color(int r, int g, int b);
};
 
#endif
