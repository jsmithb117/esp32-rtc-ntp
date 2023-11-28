#include <WiFi.h>

class WiFiManager {
private:
  const char* ssid;
  const char* password;

public:
  WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}

  void connect() {
    // Connect to WiFi
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
  }

  void loop() {
    // Check if the connection is still active, and reconnect if necessary
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connection lost. Reconnecting...");
      connect();
    }
  }

  int reportRssi() {
    // Get and return the RSSI (Received Signal Strength Indicator)
    return WiFi.RSSI();
  }
};
