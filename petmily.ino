// Preference
#include <Preferences.h>

Preferences pref;

// WebSocket
#include <WebSocketsClient.h>

// Wifi
#include <WiFi.h>

bool hasCred = false;

// Unique Chip ID
String chip_id;

// Global extern Variables
extern bool deviceConnected;
extern bool is_scale_enable;

// Local Variables for control
unsigned long lastTime = 0;  // Timer for deviceData

bool is_cover_open = false;


void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WS] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WS] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WS] get text: %s\n", payload);
      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
    case ARDUINO_EVENT_WIFI_READY:
      Serial.println("WiFi interface ready");
      break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      Serial.println("Completed scan for access points");
      break;
    case ARDUINO_EVENT_WIFI_STA_START:
      Serial.println("WiFi client started");
      break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
      Serial.println("WiFi clients stopped");
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("Connected to access point");
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Disconnected from WiFi access point");
      break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
      Serial.println("Authentication mode of access point has changed");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Obtained IP address: ");
      Serial.println(WiFi.localIP());
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      Serial.println("Lost IP address and IP address is reset to 0");
      break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:
      Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_FAILED:
      Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:
      Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_PIN:
      Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case ARDUINO_EVENT_WIFI_AP_START:
      Serial.println("WiFi access point started");
      break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
      Serial.println("WiFi access point  stopped");
      break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      Serial.println("Client connected");
      break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      Serial.println("Client disconnected");
      break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
      Serial.println("Assigned IP address to client");
      break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
      Serial.println("Received probe request");
      break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
      Serial.println("AP IPv6 is preferred");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      Serial.println("STA IPv6 is preferred");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP6:
      Serial.println("Ethernet IPv6 is preferred");
      break;
    case ARDUINO_EVENT_ETH_START:
      Serial.println("Ethernet started");
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("Ethernet stopped");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("Ethernet connected");
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("Ethernet disconnected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.println("Obtained IP address");
      break;
    default: break;
  }
}

/** Callback for receiving IP address from AP */
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));

  // Set true valid
  pref.begin("WifiCred", false);
  pref.putBool("valid", true);

  // Sync Time
  syncNTPTime();

  // Send Wifi Connect Status
  if (deviceConnected) {
    writeString("{\"wifi\": 1, \"chip_id\": \"" + String(ESP.getEfuseMac(), HEX) + "\"}");
  }
}

void setup() {
  Serial.begin(115200);

  Serial.printf("\nESP32 Unique CHIP ID: %llx\n", ESP.getEfuseMac());

  // Check has Wifi Credential
  hasCred = checkCred();

  // Initialize WebSocket
  WebSocket_setup();

  // Initalize BLE if no credential
  if (!hasCred) {
    startBLE();
  }

  // Initialize Serial for FDX-B Tag Reader
  Serial2.begin(9600);

  // Initialize HX711 Scale
  HX711_setup();

  // TEMP: Initialize Motion Sensor LED
  pinMode(13, OUTPUT);
}

void loop() {
  if (FDXB_check()) {
    if (is_cover_open) {
      is_cover_open = false;
      digitalWrite(13, 0);
    } else {
      is_cover_open = true;
      digitalWrite(13, 255);
    }
  }

  if (deviceConnected) {
    loopBLE();
  } else if (WiFi.status() == WL_CONNECTED) {
    WebSocket_loop();
    if (is_scale_enable && (millis() - lastTime) > 10000) {
      sendDeviceData();
      lastTime = millis();
    }
  }
}