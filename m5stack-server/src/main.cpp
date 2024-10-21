#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <M5Stack.h>
#include <WiFi.h>

#define XSTR(x) #x
#define STR(x) XSTR(x)

const char *WIFI_SSID = STR(WIFI_SSID);
const char *WIFI_PASSWORD = STR(WIFI_PASSWORD);

float ax, ay, az;

AsyncWebServer server(80);

void setup()
{
  delay(2000);
  Serial.begin(9600);
  delay(2000);
  Serial.printf("Connecting to %s\n", WIFI_SSID);

  WiFi.disconnect();
  delay(100);
  WiFi.setHostname("nibbler");
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

  Serial.println("Connected");
  Serial.printf("IP address: %s", WiFi.localIP().toString().c_str());

  M5.begin();
  M5.Lcd.print(WiFi.localIP());

  M5.Imu.Init();

  server.on("/gyro", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Request received");
              M5.Imu.getAccelData(&ax, &ay, &az);

              AsyncResponseStream *response = request->beginResponseStream("application/json");

              JsonDocument doc;
              doc["x"] = ax;
              doc["y"] = ay;
              doc["z"] = az;

              serializeJson(doc, *response);

              request->send(response); });

  server.begin();
}

void loop()
{
}
