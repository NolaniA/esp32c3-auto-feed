#include "ConfigClient.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

String httpGet(const String& url) {
  if (WiFi.status() != WL_CONNECTED) return "";

  HTTPClient http;
  http.begin(url);
  http.addHeader("Accept", "application/json");

  int code = http.GET();
  String body = http.getString();
  http.end();

  if (code == 200) return body;
  return "";
}

Config fetchFeedConfig(const String& url) {
  Config out{};
  out.count = 0;

  String json = httpGet(url);
  if (json.length() == 0) return out;

  StaticJsonDocument<2048> doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) {
    Serial.print("deserializeJson failed: ");
    Serial.println(err.c_str());
    return out;
  }

  if (!doc.is<JsonArray>()) {
    Serial.println("JSON is not array");
    return out;
  }

  JsonArray arr = doc.as<JsonArray>();
  int n = min((int)arr.size(), MAX_FEEDS);

  for (int i = 0; i < n; i++) {
    JsonObject o = arr[i];

    out.feeds[i].hour = o["hour"] | 0;
    out.feeds[i].minute = o["minute"] | 0;
    out.feeds[i].duration_sec = o["duration_sec"] | 0;

    out.count++;
  }

  Serial.printf("CONFIG rows=%d (use=%d)\n", (int)arr.size(), out.count);

  return out;
}
