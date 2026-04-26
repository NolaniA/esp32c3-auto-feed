#include <Arduino.h>
#include <WiFi.h>

#include "AppConfig.h"
#include "ConfigClient.h"
#include "Feeder.h"
#include "SetupPortal.h"
#include "Storage.h"
#include "TimeHelper.h"
#include "WiFiHelper.h"

String wifiSsid = "";
String wifiPass = "";
String apiUrl = "";

Config cfg{};

unsigned long lastApiTime = 0;
unsigned long lastSecondCheck = 0;
bool justFed = false;

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("Start ESP32-C3 auto feed!");

  if (!loadStoredConfig(wifiSsid, wifiPass, apiUrl)) {
    Serial.println("No saved config. Start setup AP mode.");
    startAPMode();
    return;
  }

  if (!connectWiFiCustom(wifiSsid, wifiPass)) {
    Serial.println("Cannot connect saved WiFi. Start setup AP mode.");
    startAPMode();
    return;
  }

  setupDeviceTime();
  setupFeeder(SERVO_PIN);

  // Fetch config immediately on first loop.
  lastApiTime = millis() - API_INTERVAL_MS;
}

void loop() {
  if (isAPMode()) {
    handlePortalLoop();
    return;
  }

  if (WiFi.status() != WL_CONNECTED) return;

  unsigned long now = millis();

  // Fetch feed config every 5 minutes.
  if (now - lastApiTime >= API_INTERVAL_MS) {
    lastApiTime = now;

    Config newCfg = fetchFeedConfig(apiUrl);
    if (newCfg.count > 0) {
      cfg = newCfg;
      Serial.println("CONFIG UPDATED");
    } else {
      Serial.println("KEEP OLD CONFIG");
    }
  }

  // Check feed time every 1 second.
  if (now - lastSecondCheck >= SECOND_CHECK_MS) {
    lastSecondCheck = now;

    struct tm timeinfo;
    if (!getDeviceTime(timeinfo)) return;

    Serial.println(&timeinfo);

    int H = timeinfo.tm_hour;
    int M = timeinfo.tm_min;
    int totalMin = H * 60 + M;

    int duration = 0;

    for (int i = 0; i < cfg.count; i++) {
      int feedMin = cfg.feeds[i].hour * 60 + cfg.feeds[i].minute;
      if (feedMin == totalMin) {
        duration = cfg.feeds[i].duration_sec;
        break;
      }
    }

    // Feed only once per minute.
    if (duration > 0) {
      static int lastFeedMin = -1;

      if (lastFeedMin != totalMin) {
        lastFeedMin = totalMin;
        justFed = true;
        doFeed(duration);
      }
    }

    // Sleep only if it is not immediately after feeding.
    if (!justFed) {
      if (H >= SLEEP_TIME_HOUR || H < WAKE_UP_HOUR) {
        int nowMin = H * 60 + M;
        int wakeMin = WAKE_UP_HOUR * 60;

        int minuteSleep;
        if (nowMin <= wakeMin) {
          minuteSleep = wakeMin - nowMin;
        } else {
          minuteSleep = (24 * 60 - nowMin) + wakeMin;
        }

        Serial.printf("Ready to sleep: %d minute\n", minuteSleep);
        Serial.flush();

        esp_sleep_enable_timer_wakeup((uint64_t)minuteSleep * 60ULL * 1000000ULL);
        esp_deep_sleep_start();
      }
    }

    justFed = false;
  }
}
