#pragma once

typedef enum {
  Repetition_NONE = 0,            // aperiodicity
  Repetition_Milliseconds = 1,    // This event is repeated every x milliseconds
  Repetition_Seconds = 2,         // This event is repeated every x seconds
  Repetition_Minutes = 3,         // This event is repeated every x minutes
  Repetition_Hours = 4,           // This event is repeated every x hours
  Repetition_Everyday = 5,        // The event is repeated at this time every day
  Repetition_Weekly = 6,          // This event is repeated every week at this time
  Repetition_Monthly = 7,         // This event is repeated every month at this time
} Repetition_event;

typedef struct {
  uint8_t SerialPort = 0;
  uint8_t * SerialData = NULL;
  size_t DataLength = 0;
  uint8_t DataType = 0;
  uint32_t CAN_ID = 0;
  uint8_t CAN_extd = 0;
  uint32_t repetition_Time[2]={0};        // Cycle duration     repetition_Time[0]:Cycle duration    repetition_Time[1]:Current duration
} Web_Receive;
