#ifndef COLORS
#define COLORS
 
class Colors
{
  private:
      float _brightness;
      uint32_t _expandColor(uint16_t color) ;
 
  public:
      Colors(double brightness);
      String toString(int r, int g, int b, int a);
      String toString(uint32 rgba);

      int16_t get_16b_color(int r, int g, int b);
      int16_t get_16b_color_rgba(uint32 rgba, int *a);

      uint32_t ColorHSV_32(uint16_t hue, uint8_t sat=255, uint8_t val=255);

     
};
 
#endif
