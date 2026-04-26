#include "SetupPortal.h"

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

#include "Storage.h"
#include "WebPage.h"

namespace {
WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
bool apMode = false;

void handleRoot() {
  server.send_P(200, "text/html", SETUP_HTML);
}

void handleSave() {
  String wifiSsid = server.arg("ssid");
  String wifiPass = server.arg("pass");
  String apiUrl = server.arg("api");

  saveStoredConfig(wifiSsid, wifiPass, apiUrl);

  server.send_P(200, "text/html", SAVED_HTML);

  delay(1500);
  ESP.restart();
}

void handleCaptivePortal() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send_P(200, "text/html", SETUP_HTML);
}

void handleClear() {
  server.send_P(200, "text/html", CLEARED_HTML);

  delay(1500);

  Serial.println("Clearing saved WiFi config...");
  clearStoredConfig();
  WiFi.disconnect(true, true);
  delay(500);

  ESP.restart();
}

void registerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/clear", HTTP_POST, handleClear);

  // Captive portal detection paths
  server.on("/generate_204", HTTP_GET, handleCaptivePortal);        // Android
  server.on("/gen_204", HTTP_GET, handleCaptivePortal);             // Android some devices
  server.on("/hotspot-detect.html", HTTP_GET, handleCaptivePortal); // iOS / iPadOS
  server.on("/canonical.html", HTTP_GET, handleCaptivePortal);      // Apple some versions
  server.on("/fwlink", HTTP_GET, handleCaptivePortal);              // Windows
  server.on("/connecttest.txt", HTTP_GET, handleCaptivePortal);     // Windows
  server.on("/redirect", HTTP_GET, handleCaptivePortal);            // Windows

  server.onNotFound(handleCaptivePortal);
}
}

void startAPMode() {
  apMode = true;

  WiFi.disconnect(true);
  delay(300);
  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(
    apIP,
    apIP,
    IPAddress(255, 255, 255, 0)
  );

  WiFi.softAP("ESP32-Setup", "12345678");

  Serial.println("AP Mode Started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  registerRoutes();

  server.begin();
  Serial.println("Captive portal ready");
}

bool isAPMode() {
  return apMode;
}

void handlePortalLoop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
