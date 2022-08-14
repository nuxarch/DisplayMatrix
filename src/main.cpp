#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>
#include "wifi.hpp"
#include "Fonts/kongtext4pt7b.h"

#define TZ 7 // Time Zone
#define CLK 14
#define OE 33
#define LAT 32
#define A 13 //12
#define B 19 //16
#define C 4  //17
#define D 18


_Wifi Wifi;
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 32);
// String Message0 = "To all people, I love you and wish you safety and recovery for patients";
String Message1 = "";
// String Message1 = "Politeknik Negeri Malang";
bool flasher = false;
uint8_t frameDelay = 80; // default frame delay value
static time_t last_t;
uint8_t r = 0, g = 0, b = 0;
int h, m, s, d, yr;
uint8_t month, dow;
String text;
unsigned int NewRTCh = 24;
unsigned int NewRTCm = 60;
unsigned int NewRTCs = 10;
time_t t;
char ssid[] = "Nux Arch";
char pass[] = "akhtar2015";
// char ssid[] = "Ravensburg";
// char pass[] = "4khtar2015";
const char *wd[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
const char *months[] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Ags", "Sep", "Okt", "Nop", "Des"};

// standard colors
uint16_t myRED = matrix.Color333(7, 0, 0);
uint16_t myGREEN = matrix.Color333(0, 7, 0);
uint16_t myBLUE = matrix.Color333(0, 0, 7);
uint16_t myWHITE = matrix.Color333(7, 7, 7);
uint16_t myYELLOW = matrix.Color333(7, 7, 0);
uint16_t myCYAN = matrix.Color333(0, 7, 7);
uint16_t myMAGENTA = matrix.Color333(7, 0, 7);
uint16_t myShadow = matrix.Color333(4, 0, 7);
uint16_t myROSE = matrix.Color333(7, 0, 4);
uint16_t myBLACK = matrix.Color333(0, 0, 0);
uint16_t myCOLORS[9] = {myRED, myGREEN, myWHITE, myMAGENTA, myBLUE, myYELLOW, myCYAN, myShadow, myROSE};
void getTim()
{
  if (flasher)
  {
    matrix.fillRect(54, 10, 10, 6, myWHITE);
    matrix.setCursor(10, 17);
    matrix.setTextSize(2);
    matrix.setTextColor(myCOLORS[g]);
    matrix.print(":");
    b = random(0, 8);
  }
  else
  {
    // matrix.fillRect(24, 12, 2, 6, myRED);
    matrix.setCursor(54, 10);
    matrix.setTextSize(1);
    matrix.setFont(&kongtext4pt7b);
    matrix.setTextColor(myCOLORS[b]);
    matrix.print("*");
    matrix.setFont();
    g = random(0, 8);
  }
  if (NewRTCs != s / 10)
  {
    matrix.fillRect(51, 17, 13, 6, myBLUE);
    NewRTCs = s / 10;
  }
  else
  {
    matrix.fillRect(58, 17, 6, 6, myGREEN);
  }
  matrix.setCursor(51, 16);
  matrix.setTextSize(1);
  matrix.setTextColor(myCOLORS[r]);
  matrix.setFont(&kongtext4pt7b);
  matrix.printf("%02d", s);
  matrix.setFont();
  if (NewRTCm != m)
  {
    matrix.setCursor(17, 16);
    // matrix.setFont(&FreeMonoBold12pt7b);
    matrix.setFont(&kongtext4pt7b);
    matrix.setTextColor(myMAGENTA);
    matrix.fillRect(17, 16, 13, 8, myBLACK);
    matrix.printf("%02d", m);
    matrix.setFont();
    NewRTCm = m;
  }
  if (NewRTCh != h)
  {
    matrix.setCursor(0, 16);
    // matrix.setFont(&FreeMonoBold12pt7b);
    matrix.setFont(&kongtext4pt7b);
    matrix.setTextColor(myMAGENTA);
    matrix.fillRect(0, 17, 13, 8, myBLACK);
    matrix.printf("%02d", h);
    matrix.setFont();
    NewRTCh = h;
  }
}

void Date_get()
{
  
  Serial.println("["+String(wd[dow])+", "+String(d)+" "+String(months[month])+" "+String(yr)+"]");
  Message1 = ""+String(wd[dow])+", "+String(d)+" "+String(months[month])+" "+String(yr)+" PRODI TPPU";
}
void dofw_text()
{
  text = wd[dow];
  uint16_t text_length = text.length();
  int xpo = (64 - text_length * 7) / 2;
  matrix.setCursor(xpo, 0);
  matrix.fillRect(0, 0, 64, 8, myBLACK);
  matrix.setFont(&kongtext4pt7b);
  for (int y = 0; y < 9; y++)
  {
    r = random(0, 8);
    matrix.setTextColor(myCOLORS[r]);
    matrix.print(text[y]);
  }
  matrix.setFont();
}
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text)
{
  uint16_t text_length = text.length();
  // Asuming 14 pixel average character width
  for (int xpos = 64; xpos > -(64 + text_length * 14); xpos--)
  {
    int y = r;
    matrix.setCursor(xpos, ypos);
    matrix.fillRect(0, 8, 64, 15, myBLACK);
    matrix.setFont(&kongtext4pt7b);
    for (int x = 0; x < text_length; x++)
    {
      matrix.setTextColor(myCOLORS[y]);
      matrix.print(text[x]);
      y++;
      if (y >= 9)
      {
        y = 0;
      }
    }
    matrix.setFont();
    delay(scroll_delay);
    yield();
  }
  NewRTCm = 60;
  NewRTCh = 24;
}
void updateTime()
{
  struct tm *tm;
  tm = localtime(&t);
  h = tm->tm_hour;
  m = tm->tm_min;
  s = tm->tm_sec;
  if (d != tm->tm_mday || month != tm->tm_mon || yr != tm->tm_year + 1900)
  {
    d = tm->tm_mday;
    dow = tm->tm_wday;
    month = tm->tm_mon;
    yr = tm->tm_year + 1900;
  }
}
void scroll_text1(uint8_t ypos, unsigned long scroll_delay, String text)
{
  uint16_t text_length = text.length();
  // Asuming 5 pixel average character width
  for (int xpos = 32; xpos > -(32 + text_length * 7); xpos--)
  {
    int y = r;
    matrix.setCursor(xpos, ypos);
    matrix.fillRect(0, ypos, 32, 8, myBLACK);
    matrix.setFont(&kongtext4pt7b);
    for (int x = 0; x < text_length; x++)
    {
      matrix.setTextColor(myCOLORS[y]);
      matrix.print(text[x]);
      y++;
      if (y >= 9)
      {
        y = 0;
      }
    }
    matrix.setFont();
    delay(scroll_delay);
    yield();
    t = time(NULL);
    if (last_t != t)
    {
      updateTime();
      getTim();
      flasher = !flasher;
      last_t = t;
    }
  }
}
void setup()
{
  Serial.begin(115200);

  // Serial.print("Attempting to connect to Network named: ");
  // Serial.println(ssid); // print the network name (SSID);

  Wifi.connect();
  // WiFi.begin(ssid, pass);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print(".");
  //   delay(1000);
  // }
  // Serial.println("");
  // IPAddress ip = WiFi.localIP();
  // Serial.print("IP Address: ");
  // Serial.println(ip);

  configTime(TZ * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); // enable NTP

  matrix.begin();            // setup the LED matrix
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
  matrix.fillScreen(0);

}

void loop()
{
  updateTime();
  getTim();
  Date_get();
  scroll_text1(0, frameDelay, Message1);
}
