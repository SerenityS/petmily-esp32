// WebSocket
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

void WebSocket_setup() {
  Serial.println("Initialize WebSocket...");

  // server address, port and URL
  webSocket.begin("ys.qwertycvb.com", 10115, "/ws/" + String(ESP.getEfuseMac()));

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
}

void WebSocket_loop() {
  webSocket.loop();
}