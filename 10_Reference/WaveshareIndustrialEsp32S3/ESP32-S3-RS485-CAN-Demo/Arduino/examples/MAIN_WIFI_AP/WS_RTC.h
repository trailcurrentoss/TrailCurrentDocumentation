#pragma once
#include <cstring>  // for strstr and memmove
#include "WS_Struct.h"
#include "WS_WIFI.h"
#include "WS_PCF85063.h"
#include "WS_CAN.h"
#include "WS_RS485.h"

#define Timing_events_Number_MAX 10  // Indicates the number of timers that can be set
typedef struct {
  bool Enable_Flag;                   // The timer event enabled flag.
  uint8_t Event_Number;               // Current event sequence number
  datetime_t Time;                    // Execution date
  Web_Receive Web_Data;               // Execution content
  Repetition_event repetition_State;  // Periodic execution
} Timing_RTC;

extern uint8_t Timing_events_Num;
extern Timing_RTC CHx_State[Timing_events_Number_MAX];
extern char Event_str[Timing_events_Number_MAX][1000];

void RTCTask(void* parameter);
void Continuous_Task(void *parameter);
void TimerEvent_handling(Timing_RTC event);
void TimerEvent_printf(Timing_RTC event);
void TimerEvent_Del(Timing_RTC event);

void RTC_Init(void);
void TimerEvent_Serial_Set(datetime_t time, Web_Receive* SerialData, Repetition_event Repetition);
void TimerEvent_printf_ALL(void);
void TimerEvent_Del_Number(uint8_t Event_Number);