#include <HTTPClient.h>
#include "HX711.h"
#include <Preferences.h>
#include <time.h>

String deviceDataUrl = "http://ys.qwertycvb.com:10115/petmily/device_data";
String scheduleUrl = "http://ys.qwertycvb.com:10115/schedule";

extern HX711 scale;

void postDeviceData() {
  HTTPClient http;

  http.begin(deviceDataUrl);
  http.addHeader("Content-Type", "application/json");

  String deviceDataJson = "{\"chip_id\": \"" + String(ESP.getEfuseMac(), HEX) + "\", \"bowl_amount\": " + scale.get_units() + ", \"feed_box_amount\": 1000}";

  int httpResponseCode = http.POST(deviceDataJson);

  if (httpResponseCode > 0) {
    String response = http.getString();

    Serial.print("postDeviceData: ");
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void getSchedule() {
  HTTPClient http;
  StaticJsonDocument<200> doc;

  http.begin((scheduleUrl + "?chip_id=" + String(ESP.getEfuseMac(), HEX).c_str()));

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("getSchedule: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);

    pref.begin("schedule", false);
    pref.putString("scheduleJson", payload);
    pref.end();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}