#include <Arduino.h>
#include <Preferences.h>
Preferences prefs;

// #include <WiFi.h>
// #include <PubSubClient.h>
// #include "wifi.hpp"
#include "led_matrix.hpp"
// #include "mqqt.hpp"
#include "ESP32Time.h"

#include "BluetoothSerial.h"

ESP32Time rtc(0); // offset in seconds GMT+1
// _Wifi Wifi;
P10_MatrixLed matDisplay;
// mqqt myMQQT;
BluetoothSerial ESP_BT; // Object for Bluetooth

xTaskHandle taskMQQTLoopHandle;
long now = 0;
long timeNow = 0;

void tasWaitforBT(void *parameter)
{
  for (;;)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int wait = 0;
String mymsg;

void getBTTime()
{
  prefs.begin("my-app", false);
  ESP_BT.begin("LedMatrix_00"); // Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  while (1)
  {
    // Serial.println("waiting for connection in " + String(wait) + "s");
    if (wait > 15)
    {
      break;
    }
    if (ESP_BT.available()) // Check if we receive anything from Bluetooth
    {
      mymsg = ESP_BT.readStringUntil('\n');
      if (mymsg.length() > 0)
      {
        Serial.println("Received:" + mymsg);
        Serial.println("writing: " + mymsg);
        prefs.putString("msg", "test");
        wait = 0;
      }
    }
    if (!ESP_BT.connected())
    {
      ++wait;
      delay(1000);
    }
  }
  Serial.println("bluetooth option finished,msg ==>" + prefs.getString("msg", "Jurusan TPPU"));
  Serial.println("done");
}
void setup()
{
  Serial.begin(115200);
  getBTTime();
  // rtc.setTime(00, 51, 00, 6, 11, 2022); // 17th Jan 2021 15:24:30
  // Wifi.connect();
  // configTime(TZ * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); // enable NTP
  // matrix.begin();
  // matrix.setTextWrap(false);
  // matrix.fillScreen(0);

  // client.setServer(mqtt_broker, mqtt_port);
  // client.setCallback(callback);
  // myMQQT.reconnect();
  // xTaskCreate(taskMQQTLoop, "task MQQT Loop", 6000, NULL, 0, &taskMQQTLoopHandle);
}
void loop()
{
  // Incoming = ESP_BT.read(); // Read what we recevive

  // matDisplay.updateTime();
  // matDisplay.getTim();

  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S")); // (String) returns time with specified format
  String tmp = rtc.getTime("%A, %B %d %Y %H:%M:%S");
  struct tm timeinfo = rtc.getTimeStruct();
  // matDisplay.Date_get_internal_rtc(timeinfo);
  // matDisplay.Date_get();
  // matDisplay.scroll_text1(0, frameDelay, Message1);
}
