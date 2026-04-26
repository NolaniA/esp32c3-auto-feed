#pragma once

#include <Arduino.h>

bool loadStoredConfig(String& ssid, String& pass, String& apiUrl);
void saveStoredConfig(const String& ssid, const String& pass, const String& apiUrl);
void clearStoredConfig();
