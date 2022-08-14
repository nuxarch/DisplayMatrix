#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "wifi.hpp"
#include "led_matrix.hpp"
#include "mqqt.hpp"
_Wifi Wifi;
P10_MatrixLed matDisplay;
mqqt myMQQT;
xTaskHandle taskMQQTLoopHandle;
long now = 0;
long timeNow = 0;
void taskMQQTLoop(void *parameter)
{
  for (;;)
  {
    if (!client.connected())
    {
      now = millis();
      if (now - lastReconnectAttempt > 2000)
      {
        Serial.println("Connecting to client...");
        myMQQT.reconnect(); // Attempt to reconnect
        lastReconnectAttempt = now;
      }
    }
    else
    {
      // Client connected
      client.loop();
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void setup()
{
  Serial.begin(115200);
  Wifi.connect();

  configTime(TZ * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); // enable NTP
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.fillScreen(0);

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  myMQQT.reconnect();
  xTaskCreate(taskMQQTLoop, "task MQQT Loop", 6000, NULL, 0, &taskMQQTLoopHandle);
}
void loop()
{
  matDisplay.updateTime();
  matDisplay.getTim();
  matDisplay.Date_get();
  matDisplay.scroll_text1(0, frameDelay, text);
}
