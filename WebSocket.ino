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

void sendDeviceData() {
  Serial.println("ws: Send Device Data");
  webSocket.sendTXT("{\"cmd\": \"device_data\", \"chip_id\": \"" + String(ESP.getEfuseMac(), HEX) + "\", \"bowl_amount\": " + scale.get_units() + ", \"feed_box_amount\": 1000}");
}