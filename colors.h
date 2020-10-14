#ifndef COLORS
#define COLORS
 
class Colors
{
  private:
      double _brightness;
 
  public:
      Colors(double brightness);

      int16_t get_matrix_color(int r, int g, int b);
      int16_t get_matrix_color_rgba(uint32 rgba, int *a);
      
     
};
 
#endif
