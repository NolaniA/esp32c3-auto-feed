#include "WiFiHelper.h"

#include <WiFi.h>

bool connectWiFiCustom(const String& ssid, const String& pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.print("Connecting to saved WiFi");

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("\nSaved WiFi connect failed");
  return false;
}
