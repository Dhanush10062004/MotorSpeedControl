#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "17225084-7f9e-452f-b6ae-d10bbc7ccf82"
#define CHARACTERISTIC_UUID_1 "3424af38-3e4a-45d8-91c5-4cd4687078bc"
#define CHARACTERISTIC_UUID_2 "3424af38-3e4a-45d8-91c5-4cd4687078bd"

int en_1 = 5;
int en_2 = 6;
int in_1 = 7;
int in_2 = 8;
int in_3 = 9;
int in_4 = 10;
int value_1frompage = 0;
int value_2frompage = 0;
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic_1 = NULL;
BLECharacteristic *pCharacteristic_2 = NULL;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("Connected");
  }

  void onDisconnect(BLEServer *pServer) {
    Serial.println("Disconnected");
  }
};

class MyCallbacks_1 : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string val = pCharacteristic->getValue();
    if (val.length() > 0) {
      int value = val[0];
      analogWrite(en_1, value);
    }

    if (value_1frompage == 1) {
      digitalWrite(in_1, HIGH);
      digitalWrite(in_2, LOW);
    } else if (value_1frompage == 0) {
      digitalWrite(in_1, LOW);
      digitalWrite(in_2, LOW);
    }
  }
};

class MyCallbacks_2 : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string val = pCharacteristic->getValue();
    if (val.length() > 0) {
      int value = val[0];
      analogWrite(en_2, value);
    }

    if (value_2frompage == 1) {
      digitalWrite(in_3, HIGH);
      digitalWrite(in_4, LOW);
    } else if (value_2frompage == 0) {
      digitalWrite(in_3, LOW);
      digitalWrite(in_4, LOW);
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(en_1, OUTPUT);
  pinMode(en_2, OUTPUT);
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);

  BLEDevice::init("MOTOR_CONTROL");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic_1 = pService->createCharacteristic(
      CHARACTERISTIC_UUID_1, BLECharacteristic::PROPERTY_READ |
                                  BLECharacteristic::PROPERTY_WRITE |
                                  BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_1->setCallbacks(new MyCallbacks_1());

  pCharacteristic_2 = pService->createCharacteristic(
      CHARACTERISTIC_UUID_2, BLECharacteristic::PROPERTY_READ |
                                  BLECharacteristic::PROPERTY_WRITE |
                                  BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_2->setCallbacks(new MyCallbacks_2());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify");
}

void loop() {
  
}
