# AutoFeederSplit

This is the split-file version of the original ESP32-C3 auto feeder sketch.

## Files

- `AutoFeederSplit.ino` - main program flow
- `AppConfig.h` - constants and shared structs
- `WebPage.h` - HTML pages stored in PROGMEM
- `SetupPortal.h/.cpp` - AP mode, captive portal, web routes
- `Storage.h/.cpp` - Preferences save/load/clear
- `WiFiHelper.h/.cpp` - saved WiFi connection
- `ConfigClient.h/.cpp` - HTTP GET and JSON feed config parsing
- `TimeHelper.h/.cpp` - NTP setup and local time helper
- `Feeder.h/.cpp` - servo feeder control

## Arduino IDE usage

1. Create a folder named `AutoFeederSplit`.
2. Put all files in that folder.
3. Open `AutoFeederSplit.ino` in Arduino IDE.
4. Install required libraries:
   - ESP32Servo
   - ArduinoJson
5. Select your ESP32-C3 board and upload.
