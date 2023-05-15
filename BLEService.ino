// ArduinoJSon
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// BLE
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID "7e1ba1a3-1bf8-460a-b13f-22b449163892"
#define CHARACTERISTIC_UUID_RX "11e787ed-7900-4594-9180-4699986ad978"
#define CHARACTERISTIC_UUID_TX "fe82f81a-ca0c-47d1-ac7d-b35901796fad"


class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

      Serial.println();
      StaticJsonDocument<200> doc;

      DeserializationError error = deserializeJson(doc, rxValue);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char *cmd = doc["doc"];
      const char *ssid = doc["ssid"];
      const char *pw = doc["pw"];

      Serial.println();
      Serial.println("*********");

      // Save Wifi Credentials
      pref.begin("WifiCred", false);
      pref.putString("ssid", ssid);
      pref.putString("pw", pw);
      pref.putBool("valid", false);

      connectWifi(ssid, pw);
    }
  }
};


void startBLE() {
  // Create the BLE Device
  BLEDevice::init("Petmily");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loopBLE() {
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}

void writeInt(int i) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(i);
    pTxCharacteristic->notify();
  }
}

void writeString(String str) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(str.c_str());
    pTxCharacteristic->notify();
  }
}
