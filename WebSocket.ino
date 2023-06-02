// HX711
#include "HX711.h"

// WebSocket
#include <WebSocketsClient.h>

extern HX711 scale;

WebSocketsClient webSocket;

void WebSocket_setup() {
  Serial.println("Initialize WebSocket...");

  // server address, port and URL
  webSocket.begin("ys.qwertycvb.com", 10115, "/ws/" + String(ESP.getEfuseMac(), HEX));

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 1000 again if connection has failed
  webSocket.setReconnectInterval(1000);
}

void WebSocket_loop() {
  webSocket.loop();
}

void runCmd(uint8_t* payload){
  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String command = doc["command"];

  if(strcmp(command.c_str(), "feed") == 0) {
    int feedAmount = doc["feed_amount"];
    // TODO: Implement Feeding
  } else if (strcmp(command.c_str(), "schedule") == 0) {
    getSchedule();
  }
}