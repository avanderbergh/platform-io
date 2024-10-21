#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <M5StickC.h>
#include <WiFi.h>
#include <LittleFS.h>

#include "DHT12.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

DHT12 dht12;

const char *WIFI_SSID = STR(WIFI_SSID);
const char *WIFI_PASSWORD = STR(WIFI_PASSWORD);

AsyncWebServer server(80);

void setup()
{
  M5.begin();
  Wire.begin(0, 26);

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextPadding(20);
  M5.Lcd.setCursor(20, 20);

  WiFi.disconnect();
  delay(100);
  WiFi.setHostname("zapp");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("Connection failed");
      return;
    }
  }

  M5.Lcd.print(WiFi.localIP());

  if (!LittleFS.begin())
  {
    Serial.println("An error occurred during LittleFS initialization");
    M5.Lcd.println("LittleFS error");
    return;
  }

  server.on("/tmp", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              float tmp = dht12.readTemperature();
              request->send(200, "text/plain", String(tmp)); });

  server.on("/hum", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              float hum = dht12.readHumidity();
              request->send(200, "text/plain", String(hum)); });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.begin();
}

void loop()
{
}
