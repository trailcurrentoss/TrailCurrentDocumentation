#pragma once

#include "driver/twai.h"
#include "WS_GPIO.h"
#include "WS_PCF85063.h"
// Interval:
#define TRANSMIT_RATE_MS      1000
// Interval:
#define POLLING_RATE_MS       1000

#define Communication_failure_Enable    1         // If the CAN bus is faulty for a long time, determine whether to forcibly exit

#if Communication_failure_Enable
  #define BUS_ERROR_INTERVAL_MS         5000      // Send a message every 2 seconds (2000 ms)
#endif

#define CAN_Received_Len_MAX 1000  // Indicates the number of timers that can be set

typedef struct {
  datetime_t Time;                    // data time generator
  uint32_t CAN_ID = 0;
  uint8_t CAN_extd = 0;
  uint8_t* Read_Data;
  size_t DataLength;
} CAN_Receive;

extern char * CAN_Read_Data;
extern size_t CAN_Received_Len;
extern uint32_t CAN_bitrate_kbps;
void CAN_Init(void);
void CAN_UpdateRate(uint32_t bitrate_kbps);
void CAN_Loop(void);
void CANTask(void *parameter);
void send_message_Bytes(twai_message_t message);
void send_message(uint32_t CAN_ID, uint8_t* Data, uint8_t Data_length, bool Frame_type);
esp_err_t CAN_Set_Bitrate(uint32_t bitrate_kbps);