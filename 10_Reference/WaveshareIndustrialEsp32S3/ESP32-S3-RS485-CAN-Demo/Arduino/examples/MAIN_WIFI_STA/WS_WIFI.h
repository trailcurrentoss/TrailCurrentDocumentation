#pragma once

#include "stdio.h"
#include <stdint.h>
#include <WiFi.h>
#include <WebServer.h> 
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "WS_PCF85063.h"
#include "WS_Information.h"
#include "WS_Struct.h"
#include "WS_RTC.h"
#include "WS_RS485.h"
#include "WS_CAN.h"

extern char ipStr[16];

void handleGetRS485Data();
void handleRS485Send();
void handleGetCANData();
void handleCANSend();

void handleSwitch1();
void WIFI_Init();
void WifiStaTask(void *parameter);

bool ParseRTCData(const char* Text, datetime_t* dt, Web_Receive* SerialData, Repetition_event* cycleEvent);
bool ParseRtcConfig(const char* Text, datetime_t* dt);
bool ParseRS485Config(const char* Text,uint8_t* RS485_Read_Data_Type);
bool ParseRS485Data(const char* Text, RS485_Receive* RS485Data);
bool ParseCANData(const char* Text, CAN_Receive* CANData) ; 
bool ParseRS485BaudRateConfig(const char* Text,  unsigned long * RS485_BaudRate);
bool ParseCANRateConfig(const char* Text,  uint32_t * CAN_bitrate_kbps);




