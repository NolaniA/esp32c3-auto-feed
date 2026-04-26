#pragma once

#include <Arduino.h>

#include "AppConfig.h"

String httpGet(const String& url);
Config fetchFeedConfig(const String& url);
