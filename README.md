This program use P10 RGB Matrix Panel and ESP32 to display information
First line is running text displaying Network Time Protocol : days of a week and, date, month and year
Second line display hour and minutes

make sure this is configured in main.cpp
#define TZ 7 // Time Zone
#define CLK 14
#define OE 33
#define LAT 32
#define A 13 //12
#define B 19 //16
#define C 4  //17
#define D 18
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 32);

also modify in RGBmatrixPanel.cpp
// R1, G1, B1, R2, G2, B2 pins
line 143 :  static const uint8_t defaultrgbpins[] = {25, 26, 27, 21, 22, 23};
