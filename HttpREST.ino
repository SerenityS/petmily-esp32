#include <HTTPClient.h>
#include "HX711.h"
#include <Preferences.h>
#include <time.h>

String deviceDataUrl = "http://ys.qwertycvb.com:10115/petmily/device_data";
String historyUrl = "http://ys.qwertycvb.com:10115/history";
String scheduleUrl = "http://ys.qwertycvb.com:10115/schedule";

extern HX711 bowl_scale;
extern HX711 feed_box_scale;

void postDeviceData() {
  HTTPClient http;

  http.begin(deviceDataUrl);
  http.addHeader("Content-Type", "application/json");

  String deviceDataJson = "{\"chip_id\": \"" + String(ESP.getEfuseMac(), HEX) + "\", \"bowl_amount\": " + bowl_scale.get_units(3) * 1000 + ", \"feed_box_amount\":" + feed_box_scale.get_units(3) * 1000 + "}";

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

void postHistory(int consume) {
  HTTPClient http;

  http.begin(historyUrl);
  http.addHeader("Content-Type", "application/json");

  String historyJson = "{\"user_id\": \"069cda42-2c51-414d-ba22-a7e4ec49bff8\", \"chip_id\": \"" + String(ESP.getEfuseMac(), HEX) + "\", \"consume\": " + consume + "}";

  int httpResponseCode = http.POST(historyJson);

  if (httpResponseCode > 0) {
    String response = http.getString();

    Serial.print("postHistory: ");
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