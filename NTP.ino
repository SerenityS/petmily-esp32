#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 32400;  // KST(+9)
const int daylightOffset_sec = 0;

void ntpPrintTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void syncNTPTime() {
  Serial.println("Sync Time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}