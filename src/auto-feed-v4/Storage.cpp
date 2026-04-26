#include "Storage.h"

#include <Preferences.h>

namespace {
const char* NAMESPACE = "config";
const char* KEY_SSID = "ssid";
const char* KEY_PASS = "pass";
const char* KEY_API = "api";
}

bool loadStoredConfig(String& ssid, String& pass, String& apiUrl) {
  Preferences preferences;
  preferences.begin(NAMESPACE, true);

  ssid = preferences.getString(KEY_SSID, "");
  pass = preferences.getString(KEY_PASS, "");
  apiUrl = preferences.getString(KEY_API, "");

  preferences.end();

  return ssid.length() > 0;
}

void saveStoredConfig(const String& ssid, const String& pass, const String& apiUrl) {
  Preferences preferences;
  preferences.begin(NAMESPACE, false);

  preferences.putString(KEY_SSID, ssid);
  preferences.putString(KEY_PASS, pass);
  preferences.putString(KEY_API, apiUrl);

  preferences.end();
}

void clearStoredConfig() {
  Preferences preferences;
  preferences.begin(NAMESPACE, false);
  preferences.clear();
  preferences.end();
}
