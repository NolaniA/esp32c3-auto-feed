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

struct FeedTime {
    int hour;
    int minute;
    int duration_sec;
  };  

struct Config {
    FeedTime feed_time1;
    FeedTime feed_time2;
    FeedTime feed_time3;
  };

Config cfg;

unsigned long last_api_time = 0;
const unsigned long api_interval = 5 * 60 * 1000;   // 5 นาที



//----------------------------------
// function
//----------------------------------


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
Config http_get_config(String url) {
  Config cfg;

  String json = http_get(url);
  StaticJsonDocument<512> doc;
  deserializeJson(doc, json);

  cfg.feed_time1 = {
    doc["feed_time1"]["hour"],
    doc["feed_time1"]["minute"],
    doc["feed_time1"]["duration_sec"]
  };

  cfg.feed_time2 = {
    doc["feed_time2"]["hour"],
    doc["feed_time2"]["minute"],
    doc["feed_time2"]["duration_sec"]
  };

  cfg.feed_time3 = {
    doc["feed_time3"]["hour"],
    doc["feed_time3"]["minute"],
    doc["feed_time3"]["duration_sec"]
  };

  return cfg;
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


bool getTimeInfo(struct tm &timeinfo) {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return false;
  }

  return true;
}



void setup() {
  Serial.begin(115200);

  delay(300);

  // -------say hi first time -------
  Serial.println("Start ESP32-C3 auto feed");

  connectWiFi();

  pinMode(8, OUTPUT);

  last_api_time = millis() - api_interval;

}

void loop() {

  unsigned long now = millis();

  // ===== ยิง API ทุก ๆ 5 นาที =====
  if (now - last_api_time >= api_interval) {
    last_api_time = now;

    cfg = http_get_config(CONFIG_URL);

  }


  static int last_feed_minute = -1;

  struct tm timeinfo;
  if (getTimeInfo(timeinfo)) {
    Serial.println(&timeinfo);
    int current_hour   = timeinfo.tm_hour;
    int current_minute = timeinfo.tm_min;
    int duration       = 0;

    duration = (cfg.feed_time1.hour == current_hour && cfg.feed_time1.minute == current_minute)? cfg.feed_time1.duration_sec:duration;
    duration = (cfg.feed_time2.hour == current_hour && cfg.feed_time2.minute == current_minute)? cfg.feed_time2.duration_sec:duration;
    duration = (cfg.feed_time3.hour == current_hour && cfg.feed_time3.minute == current_minute)? cfg.feed_time3.duration_sec:duration;

    if (duration > 0) {
      if (last_feed_minute != current_minute) {
        last_feed_minute = current_minute;

        Serial.printf("FEED START %d sec\n", duration);
        digitalWrite(8, LOW); //led
        delay(duration * 1000);
        digitalWrite(8, HIGH);
        Serial.println("FEED DONE");

        delay((60 - timeinfo.tm_sec) * 1000);
      }
    }


  }

  delay(10 * 1000);   //second

}
