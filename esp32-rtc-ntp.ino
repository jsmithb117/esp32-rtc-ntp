#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include "isDST.h"


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Rtc_Pcf8563 rtc;

// Variables to save date and time
String formattedDate;
String formattedTime;
String dayStamp;
String timeStamp;
int lastUpdateDate = 0;

void updateRTCwithNTP() {
  // Get NTP time
  formattedDate = timeClient.getFormattedDate();
  Serial.print("Formatted NTP Date: ");
  Serial.println(formattedDate);

  // Format NTP time and extract bytes: twoDigitYear, month, dayOfMonth, dayOfWeek, century
  int splitT = formattedTime.indexOf("T");
  byte fourDigitYear = formattedDate.substring(0, 4).toInt();
  byte month = formattedDate.substring(5, 7).toInt();
  byte dayOfMonth = formattedDate.substring(8, 10).toInt();
  byte dayOfWeek = timeClient.getDay();
  byte century = 0; // 0 represents 2000, 1 represents 1900

  // Update offset for DaylightSavingsTime as necessary
  // if it is before the second Sunday in March at 2am, or after the first Sunday in November at 2am
  // then it is not DaylightSavingsTime

  if (isDST(fourDigitYear, month, dayOfMonth, timeClient.getHours())) {
    timeClient.setTimeOffset(-25200);
  } else {
    timeClient.setTimeOffset(-28800);
  }

  rtc.setDate(dayOfMonth, dayOfWeek, month, century, fourDigitYear - 2000);
  rtc.setTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
}

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

  // Initialize NetworkTimeProtocol (NTP) Client
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT = 0
  // GMT +1 = 3600
  // GMT -7 = -25200 (Pacific Daylight Time)
  // GMT -8 = -28800 (Pacific Standard Time)
  // timeClient.setTimeOffset(-28800);
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // Initialize RealTimeClock (RTC) and update with NTP time
  Serial.println("RTC init");
  rtc.initClock();
  updateRTCwithNTP();
}
void loop() {
  int today = rtc.getDay();
  Serial.print(rtc.getYear());
  Serial.print("-");
  Serial.print(rtc.getMonth());
  Serial.print("-");
  Serial.print(today);
  Serial.print("T");
  Serial.print(rtc.getHour());
  Serial.print(":");
  Serial.print(rtc.getMinute());
  Serial.print(":");
  Serial.print(rtc.getSecond());
  Serial.println("Z");
  // update RTC once a day
  if (lastUpdateDate != today) {
    Serial.println("Updating RTC");
    updateRTCwithNTP();
    lastUpdateDate = today;
  } else {
    Serial.println("RTC up to date");
  }
  delay(1000);
}
