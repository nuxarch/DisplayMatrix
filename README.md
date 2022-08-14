# ESP32[esp32doit-devkit-v1] with P10 Matrix Panel Display

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)
- [Contributing](../CONTRIBUTING.md)

## About <a name = "about"></a>

This ESP32 program use P10 RGB Matrix Panel to display information
First line is running text displaying Network Time Protocol : days of a week and, date, month and year
Second line display hour and minutes

make sure this is configured in main.cpp
- #define TZ 7 // Time Zone
- #define CLK 14
- #define OE 33
- #define LAT 32
- #define A 13 //12
- #define B 19 //16
- #define C 4  //17
- #define D 18
- RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 32);

also modify in RGBmatrixPanel.cpp
// R1, G1, B1, R2, G2, B2 pins
line 143 :  static const uint8_t defaultrgbpins[] = {25, 26, 27, 21, 22, 23};



## Getting Started <a name = "getting_started"></a>

1. Clone the github 
2. Open Folder from VSCode and PIO
3. Build and Compile 

### Prerequisites
1. VSCode
2. PlatformIO
3. Library

### Installing
see Platformio.ini file

## Usage <a name = "usage"></a>
https://github.com/nuxarch

