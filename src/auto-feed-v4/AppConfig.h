#pragma once

#include <Arduino.h>

// ---------- Feed Config ----------
constexpr int MAX_FEEDS = 10;

struct FeedTime {
  int hour;
  int minute;
  int duration_sec;
};

struct Config {
  FeedTime feeds[MAX_FEEDS];
  int count;
};

// ---------- Time ----------
static constexpr const char* NTP_SERVER = "pool.ntp.org";
constexpr long GMT_OFFSET_SEC = 7 * 3600;   // GMT+7
constexpr int DAYLIGHT_OFFSET_SEC = 0;

// ---------- Timing ----------
constexpr unsigned long API_INTERVAL_MS = 5UL * 60UL * 1000UL; // 5 minutes
constexpr unsigned long SECOND_CHECK_MS = 1000UL;

// ---------- Servo ----------
constexpr int SERVO_PIN = 9;

// ---------- Sleep ----------
constexpr int SLEEP_TIME_HOUR = 21; // 21:00
constexpr int WAKE_UP_HOUR = 5;     // 05:00
