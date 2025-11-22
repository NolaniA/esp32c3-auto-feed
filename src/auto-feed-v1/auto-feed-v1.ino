#include <Arduino.h>

#include <WiFi.h>

#include <HTTPClient.h>

#include <ArduinoJson.h>

#include "time.h"


// ---- Wi-Fi ----
const char* WIFI_SSID = " GgEz";
const char* WIFI_PASS = "0989425594";

// ---- Time ---- 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600;     // GMT+7
const int   daylightOffset_sec = 0;

// ---- config
const char* CONFIG_URL = "https://nolania.github.io/esp32c3-auto-feed/config-auto-feed/config-time.json";


String http_get(String url){
  // check wifi connect
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("wifi not connect");
    return  "" ;
  }

  // define HTTPClient as http
  HTTPClient http;

  http.begin(url);

  int httpget = http.GET();

  // check status code must > 0 (200)
  if(httpget < 0){
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpget).c_str());
    return "" ;
  }

  // check api get success
  if (httpget == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("[HTTP] get success!!");
    return  payload ;
  }


}

void connectWiFi() {
  Serial.println("WiFi connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.printf("\nWiFi OK: %s\n", WiFi.localIP().toString().c_str());
}


String getConfig(String data, String key){
  
  JsonDocument dataConfig;

  DeserializationError error = deserializeJson(dataConfig, data);
  if (error) {
    Serial.println("deserializeJson() failed: ");
  }

  String update = dataConfig[key];
  return  update ;
}


bool getTimeInfo(struct tm &timeinfo) {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return false;
  }

  return true;
}

void checkLightTrigger(struct tm timeinfo) {
  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;

  // เงื่อนไขเวลาใดเวลาหนึ่ง
  bool isTriggerTime =
      (hour == 8  && minute == 0) ||
      (hour == 12 && minute == 0) ||
      (hour == 18 && minute == 0);

  if (isTriggerTime) {
    // digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON (trigger time)");
  } else {
    Serial.println("LED OFF (trigger time)");
    // digitalWrite(LED_PIN, LOW);
  }
}


void setup() {
  Serial.begin(115200);

  delay(300);

  // -------say hi first time -------
  Serial.println("Start ESP32-C3");

  connectWiFi();

}

void loop() {
  // digitalWrite(8, HIGH);

  // delay(1000);

  // digitalWrite(8, LOW);

  String data =  http_get(CONFIG_URL);
  String update =  getConfig(data, "hour_time1");


  struct tm timeinfo;
  if (getTimeInfo(timeinfo)) {
    Serial.println(&timeinfo);
  }




  
  



  delay(10 * 1000);   //second

}
