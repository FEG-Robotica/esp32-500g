/*
 * File:   main.cpp
 * Author: Hiro
 *
 * Created on 03 de Dezembro de 2021, 23:39
 */

#include <Arduino.h>
#include <sonicGPIO.h>
#include <sonicAD.h>
#include <sonicPWM.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

std::string comando = "";

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();

    if (value == "L")
    {
      comando = "LED";
    }

    if (value == "P")
    {
      comando = "Parar";
    }
  }
};

void setup()
{

  Serial.begin(9600);
  pinMode(2, OUTPUT);
  gpioConfig();
  adConfig();
  pwmConfig();

  BLEDevice::init("Sonic");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop()
{
  if (comando == "LED")
  {
    for (int i = 0; i < 100; i++)
    {
      pwmASetDuty(i);
      pwmBSetDuty(100 - i);
      delay(10);

      if (comando == "Parar")
      {
        break;
      }
    }
  }

  if (comando == "Parar")
  {
    pwmASetDuty(0);
    pwmBSetDuty(0);
  }
}