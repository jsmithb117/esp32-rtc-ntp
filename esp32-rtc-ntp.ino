#include <WiFi.h>
#include "RTCManager.h"
#include "secrets.h"
#include "WiFiManager.h"


RTCManager rtcClient;
WiFiManager wifiManager(ssid, password);


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  wifiManager.connect();
  // Initialize rtcClient
  rtcClient.init();
}
void loop() {
  wifiManager.loop();
  rtcClient.loop();

  delay(1000);
}
