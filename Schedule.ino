// Preference
#include <Preferences.h>

// time
#include <time.h>

void Schedule_setup() {
  Serial.println("Initialize Feeding Event...");
  getSchedule();
}

void Schedule_check() {
  StaticJsonDocument<1500> doc;

  struct tm localTime;

  if (!getLocalTime(&localTime)) {
    Serial.println("Failed to obtain time");
    return;
  }

  pref.begin("schedule", true);
  String scheduleJson = pref.getString("scheduleJson", "");
  pref.end();

  deserializeJson(doc, scheduleJson);
  for (JsonObject item : doc.as<JsonArray>()) {
    bool isEnable = item["is_enable"];
    String date = item["date"];
    int amount = item["amount"];

    struct tm scheduleTime = { 0 };
    strptime(date.c_str(), "%Y-%m-%dT%H:%M:%S.%03d", &scheduleTime);

    if (isEnable && localTime.tm_hour == localTime.tm_hour && localTime.tm_min == scheduleTime.tm_min) {
      Serial.print("Schedule: Run Feeding, amount:");
      Serial.println(amount);
      Servo_open();
      delay(1500);
      Servo_close();
    }
  }
}
