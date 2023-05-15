// Wifi
#include <WiFi.h>

// Wifi Credential
String ssid;
String pw;

// Status Variable
bool hasCredentials = false;
volatile bool isConnected = false;
bool connStatusChanged = false;

bool checkCred() {
  pref.begin("WifiCred", false);
  bool hasCred = pref.getBool("valid", false);
  if (hasCred) {
    ssid = pref.getString("ssid", "");
    pw = pref.getString("pw", "");

    if (ssid.equals("")
        || pw.equals("")) {
      Serial.println("Found preferences but credentials are invalid");
    } else {
      Serial.println("Read from preferences:");
      Serial.println("SSID: " + ssid + " password: " + pw);
      hasCredentials = true;

      connectWifi(ssid, pw);
    }
  } else {
    Serial.println("Could not find preferences, need send data over BLE");
  }
  pref.end();
  return hasCredentials;
}

void connectWifi(String ssid, String pw) {
  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
  },
                                       WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.disconnect(true);
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);

  Serial.print("Start Wifi Connection to ");
  Serial.print(ssid);
  Serial.println(pw);

  WiFi.begin(ssid.c_str(), pw.c_str());
}
