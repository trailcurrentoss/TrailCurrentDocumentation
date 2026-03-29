#pragma once

#include <HardwareSerial.h>     // Reference the ESP32 built-in serial port library
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "WS_RS485.h"
#include "WS_CAN.h"
#include "WS_Information.h"
#include "WS_WIFI.h"
#include "WS_RTC.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"                     // UUID of the server
#define RX_CHARACTERISTIC_UUID  "beb5483e-36e1-4688-b7f5-ea07361b26a8"          // UUID of the characteristic Tx
#define TX_CHARACTERISTIC_UUID  "beb5484a-36e1-4688-b7f5-ea07361b26a8"          // UUID of the characteristic Rx

#define Bluetooth_Mode    2


void Bluetooth_SendData(char * Data);   
void Bluetooth_Init();
void BLETask(void *parameter);
void BLE_Set_RTC_Event(uint8_t* valueBytes);