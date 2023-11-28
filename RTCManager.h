#include <Rtc_Pcf8563.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "isDST.h"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Rtc_Pcf8563 rtc;

// Create a class 'RTCManager' to manage the RTC module including setup, sync with NTP on setup and once daily, and provide public functions for getting the current time.
class RTCManager {
  public:
    void init() {
      Serial.println("NTP Client init");
      timeClient.begin();
      while(!timeClient.update()) {
        timeClient.forceUpdate();
      }
      Serial.println("RTC Client init");
      rtc.initClock();
      updateRTCwithNTP();
    }
    int getYear() { return rtc.getYear(); }
    int getMonth() { return rtc.getMonth(); }
    int getDay() { return rtc.getDay(); }
    int getHour() { return rtc.getHour(); }
    int getMinute() { return rtc.getMinute(); }
    int getSecond() { return rtc.getSecond(); }
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
    }
  private:
    int lastUpdateDate = 0;
    String formattedDate;
    String formattedTime;
    String dayStamp;
    String timeStamp;
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
};