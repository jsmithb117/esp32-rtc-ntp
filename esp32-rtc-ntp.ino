#include <WiFi.h>
#include "RTCManager.h"
#include "secrets.h"

RTCManager rtcClient;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize rtcClient
  rtcClient.init();
}
void loop() {
  rtcClient.loop();
  delay(1000);
}
