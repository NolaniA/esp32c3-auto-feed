#include "TimeHelper.h"

#include "AppConfig.h"

void setupDeviceTime() {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

bool getDeviceTime(struct tm& timeinfo) {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get time");
    return false;
  }
  return true;
}
