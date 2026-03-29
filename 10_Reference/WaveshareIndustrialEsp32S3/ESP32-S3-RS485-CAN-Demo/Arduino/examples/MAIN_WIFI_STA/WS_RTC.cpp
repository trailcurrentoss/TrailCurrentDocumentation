#include "WS_RTC.h"

Timing_RTC Events_State[Timing_events_Number_MAX];       // Set a maximum of Timing_events_Number_MAX timers
char Event_str[Timing_events_Number_MAX][1000];
static Timing_RTC Events_State_Default = {0};            // Event initial state
const unsigned char Event_cycle[8][13] = {"Aperiodicity", "Milliseconds", "Seconds", "Minutes", "Hours", "Everyday", "Weekly", "Monthly"};

uint32_t Cycle_duration = 1;
void RTC_Init(void){
  PCF85063_Init();
  xTaskCreatePinnedToCore(
    RTCTask,    
    "RTCTask",   
    4096,                
    NULL,                 
    3,                   
    NULL,                 
    0                   
  );
  
  xTaskCreatePinnedToCore(
    Continuous_Task,
    "Continuous Task",
    4096,          
    NULL,     
    3,
    NULL,
    0
  );
}
uint8_t Timing_events_Num = 0;
void RTCTask(void *parameter)
{ 
  static uint8_t Time_Old = 0;
  while(1){
    if(Timing_events_Num){
      for (int i = 0; i < Timing_events_Number_MAX; i++){
        if(Events_State[i].Enable_Flag){
          if(Events_State[i].Time.hour == datetime.hour && Events_State[i].Time.minute == datetime.minute && Events_State[i].Time.second == datetime.second && datetime.second != Time_Old){       // The event time is consistent with the current time
            switch(Events_State[i].repetition_State){
              case Repetition_NONE:
                if(Events_State[i].Time.year == datetime.year && Events_State[i].Time.month == datetime.month && Events_State[i].Time.day == datetime.day){ // Executes at the defined date and time
                  TimerEvent_handling(Events_State[i]);
                  TimerEvent_Del(Events_State[i]);
                }
                break;
              case Repetition_Everyday:
                TimerEvent_handling(Events_State[i]);
                break;
              case Repetition_Weekly:
                if(Events_State[i].Time.dotw == datetime.dotw){
                  TimerEvent_handling(Events_State[i]);
                }
                break;
              case Repetition_Monthly:
                if(Events_State[i].Time.day == datetime.day){
                  TimerEvent_handling(Events_State[i]);
                }
                break;
              case Repetition_Hours: break;
              case Repetition_Minutes: break;
              case Repetition_Seconds: break;
              case Repetition_Milliseconds: break;
              default:
                printf("Event error!!!!\n");
                break;
            }
          }
        }
      }
    }
    Time_Old = datetime.second;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  vTaskDelete(NULL);
}
void Continuous_Task(void *parameter){
  while(1){
    if(Timing_events_Num){
      for (int i = 0; i < Timing_events_Number_MAX; i++){
        if(Events_State[i].Enable_Flag && Events_State[i].Web_Data.repetition_Time[0] != 0 && (Events_State[i].repetition_State == Repetition_Hours || Events_State[i].repetition_State == Repetition_Minutes || Events_State[i].repetition_State == Repetition_Seconds || Events_State[i].repetition_State == Repetition_Milliseconds)){
          Events_State[i].Web_Data.repetition_Time[1] = Events_State[i].Web_Data.repetition_Time[1] + Cycle_duration;
          if(Events_State[i].Web_Data.repetition_Time[0] <= Events_State[i].Web_Data.repetition_Time[1]){
            Events_State[i].Web_Data.repetition_Time[1] = 0;
            TimerEvent_handling(Events_State[i]);
          }
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(Cycle_duration));
  }
  vTaskDelete(NULL);

}
void TimerEvent_handling(Timing_RTC event){         // event trigger
  uint8_t Retain_channels = 0;  
  if(event.repetition_State != Repetition_Hours && event.repetition_State != Repetition_Minutes && event.repetition_State != Repetition_Seconds && event.repetition_State != Repetition_Milliseconds)
    printf("Event %d : triggered\r\n", event.Event_Number);
  // char datetime_str[50];
  // datetime_to_str(datetime_str,event.Time);
  if(event.Web_Data.SerialPort){     // CAN 
    twai_message_t Web_message;
    Web_message.identifier = event.Web_Data.CAN_ID ;
    Web_message.extd = event.Web_Data.CAN_extd;                              // Frame_type : 1：Extended frames   0：Standard frames
    Web_message.data_length_code = event.Web_Data.DataLength;                  // valueBytes[0] to valueBytes[6] are configuration parameters
    for (int i = 0; i < Web_message.data_length_code; i++) {
      Web_message.data[i] = event.Web_Data.SerialData[i];
    }
    send_message_Bytes(Web_message);                                  // Send data from the CAN
  }
  else{                                 // RS485
    SetData(event.Web_Data.SerialData, event.Web_Data.DataLength);                            // Send data from the RS485
    // printf("RTC Event execution : %.*s\r\n", event.Web_Data.DataLength, event.Web_Data.SerialData);
  }
}
uint32_t calculate_repetition_gcd(void) {
  uint32_t gcd = 0;
  for (int i = 0; i < Timing_events_Number_MAX; i++) {
    if (Events_State[i].Enable_Flag && (Events_State[i].repetition_State == Repetition_Hours || Events_State[i].repetition_State == Repetition_Minutes || Events_State[i].repetition_State == Repetition_Seconds || Events_State[i].repetition_State == Repetition_Milliseconds)) {
      uint32_t value = Events_State[i].Web_Data.repetition_Time[0];
      if (value > 0) {
        if (gcd == 0) {
          gcd = value;
        } else {
          // 辗转相除法求最大公因数
          uint32_t a = gcd, b = value;
          while (b != 0) {
            uint32_t temp = b;
            b = a % b;
            a = temp;
          }
          gcd = a;
        }
      }
    }
  }

  return gcd;
}

void TimerEvent_Serial_Set(datetime_t time, Web_Receive* SerialData, Repetition_event Repetition)
{
  char datetime_str[50];
  datetime_to_str(datetime_str,datetime);
  printf("Now Time: %s!!!!\r\n", datetime_str);
  if(Timing_events_Num + 1 >= Timing_events_Number_MAX)
  {
    printf("Note : The number of scheduled events is full.\r\n");
    return;
  }
  else{
    Events_State[Timing_events_Num].Enable_Flag = true;
    Events_State[Timing_events_Num].Event_Number = Timing_events_Num + 1;                                       // Event Serial number
    Events_State[Timing_events_Num].Web_Data = *SerialData;
    Events_State[Timing_events_Num].Time = time;
    Events_State[Timing_events_Num].repetition_State = Repetition;
    datetime_to_str(datetime_str,time);
    printf("New timing event%d :\r\n       %s \r\n",Timing_events_Num, datetime_str);
    if(Events_State[Timing_events_Num].Web_Data.SerialPort == 0){                                                // RS485
      if(Events_State[Timing_events_Num].Web_Data.DataType){
        printf("        RS485 Send Data: hex\r\n");
        printf("        RS485 Data:");
        for(int i=0;i<Events_State[Timing_events_Num].Web_Data.DataLength;i++){
          printf(" 0x%.02X ", Events_State[Timing_events_Num].Web_Data.SerialData[i]);
          if ((i + 1) % 10 == 0 && (i + 1) < Events_State[Timing_events_Num].Web_Data.DataLength) {
            printf("\n                  ");
          }
        }
      }
      else{
        printf("        RS485 Send Data: char\r\n");
        uint8_t Web_SerialData[SerialData->DataLength+1];
        memcpy(Web_SerialData, Events_State[Timing_events_Num].Web_Data.SerialData, SerialData->DataLength);
        Web_SerialData[SerialData->DataLength] = '\0';  
        printf("        RS485 Data: %s ", Web_SerialData);
      }
    }
    else if(Events_State[Timing_events_Num].Web_Data.SerialPort == 1){                                                    // CAN
      if(Events_State[Timing_events_Num].Web_Data.DataType){
        printf("        CAN Send Data: hex\r\n");
        if(Events_State[Timing_events_Num].Web_Data.CAN_extd){
          printf("        CAN Type: Extended   CAN ID: 0x%lX \r\n", Events_State[Timing_events_Num].Web_Data.CAN_ID);
        }
        else{
          printf("        CAN Type: Standard   CAN ID: 0x%lX  \r\n", Events_State[Timing_events_Num].Web_Data.CAN_ID);
        }
        printf("        CAN Data:");
        for(int i=0;i<Events_State[Timing_events_Num].Web_Data.DataLength;i++){
          printf(" 0x%lX ", Events_State[Timing_events_Num].Web_Data.SerialData[i]);
          if ((i + 1) % 10 == 0 && (i + 1) < Events_State[Timing_events_Num].Web_Data.DataLength) {
            printf("\n                 ");
          }
        }
      }
      // else{
      //   printf("        CAN Send Data: char\r\n");
      //   if(Events_State[Timing_events_Num].Web_Data.CAN_extd){
      //     printf("        CAN Type: Extended   CAN ID: %ld \r\n", Events_State[Timing_events_Num].Web_Data.CAN_ID);
      //   }
      //   else{
      //     printf("        CAN Type: Standard   CAN ID: %ld  \r\n", Events_State[Timing_events_Num].Web_Data.CAN_ID);
      //   }
      //   uint8_t Web_SerialData[SerialData->DataLength+1];
      //   memcpy(Web_SerialData, Events_State[Timing_events_Num].Web_Data.SerialData, SerialData->DataLength);
      //   Web_SerialData[SerialData->DataLength] = '\0';  
      //   printf("        CAN Data: %s ", Web_SerialData);
      // }
    }
    printf("\r\n");
    printf("\r\n");
    if(Events_State[Timing_events_Num].repetition_State == Repetition_Hours)
      printf("        ----- %ld %s\r\n\r\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0] / 3600000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Minutes)
      printf("        ----- %ld %s\r\n\r\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0] / 60000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Seconds)
      printf("        ----- %ld %s\r\n\r\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0]/ 1000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Milliseconds)
      printf("        ----- %ld %s\r\n\r\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0], Event_cycle[Repetition]);
    else
      printf("        ----- %s\r\n\r\n", Event_cycle[Repetition]);
    printf("\r\n");
    if(Events_State[Timing_events_Num].repetition_State == Repetition_Hours || Events_State[Timing_events_Num].repetition_State == Repetition_Minutes || Events_State[Timing_events_Num].repetition_State == Repetition_Seconds || Events_State[Timing_events_Num].repetition_State == Repetition_Milliseconds){
      uint32_t Cycle_duration_GCD = calculate_repetition_gcd();
      if(Cycle_duration_GCD){
        printf("calculate repetition gcd:%ld\r\n",Cycle_duration_GCD);
        Cycle_duration = Cycle_duration_GCD; 
      }
    }
    
    int len = 0;  
    int total_len = 1000;
    char *Event_content = (char *)heap_caps_malloc(total_len, MALLOC_CAP_SPIRAM);
    if (Event_content == NULL) {
        printf("Memory allocation failed\n");
        return ;
    }
    if(SerialData->SerialPort == 0) {
      if (SerialData->DataType == 1)   // hex
        len += snprintf(Event_content + len, total_len - len, "&nbsp;&nbsp;&nbsp;&nbsp;RS485&nbsp;Send&nbsp;Data&nbsp;&nbsp;&nbsp;&nbsp;(hex):\\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
      else                                               // RS485
        len += snprintf(Event_content + len, total_len - len, "&nbsp;&nbsp;&nbsp;&nbsp;RS485&nbsp;Send&nbsp;Data&nbsp;&nbsp;&nbsp;(char):\\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    }    
    else if(SerialData->SerialPort == 1){                                                    // CAN
      if (SerialData->DataType == 1)   // hex
        len += snprintf(Event_content + len, total_len - len, "&nbsp;&nbsp;&nbsp;&nbsp;CAN&nbsp;Send&nbsp;Data&nbsp;&nbsp;(hex):\\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
      // else                                               // CAN
      //   len += snprintf(Event_content + len, total_len - len, "&nbsp;&nbsp;&nbsp;&nbsp;CAN&nbsp;Send&nbsp;Data&nbsp;(char):\\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    }   
    
    char temp[SerialData->DataLength * 12 + 1] = {0};  // 每个字节最多4字符 "0xXX "，外加结尾'\0'
    if (SerialData->DataType == 1) {  // hex
      for (int i = 0; i < SerialData->DataLength; i++) {
        char hex_byte[6];
        snprintf(hex_byte, sizeof(hex_byte), "0x%02X ", SerialData->SerialData[i]);
        strcat(temp, hex_byte);
        // 每10个字节添加换行和缩进
        if ((i + 1) % 10 == 0 && (i + 1) < SerialData->DataLength) {
            strcat(temp, "\\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
        }
      }
    } 
    else {
      memcpy(temp, SerialData->SerialData, SerialData->DataLength);
      temp[SerialData->DataLength] = '\0';
    }
    // uint8_t temp[SerialData->DataLength+1];
    // memcpy(temp, SerialData->SerialData, SerialData->DataLength);
    // temp[SerialData->DataLength] = '\0'; 
    len += snprintf(Event_content + len, total_len - len, " %s ", temp);
    if(Events_State[Timing_events_Num].repetition_State == Repetition_Hours)
      len += snprintf(Event_content + len, total_len - len, "\\n&nbsp;&nbsp;&nbsp;&nbsp;----- %ld %s\\n\\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0] / 3600000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Minutes)
      len += snprintf(Event_content + len, total_len - len, "\\n&nbsp;&nbsp;&nbsp;&nbsp;----- %ld %s\\n\\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0] / 60000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Seconds)
      len += snprintf(Event_content + len, total_len - len, "\\n&nbsp;&nbsp;&nbsp;&nbsp;----- %ld %s\\n\\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0] / 1000, Event_cycle[Repetition]);
    else if(Events_State[Timing_events_Num].repetition_State == Repetition_Milliseconds)
      len += snprintf(Event_content + len, total_len - len, "\\n&nbsp;&nbsp;&nbsp;&nbsp;----- %ld %s\\n\\n", Events_State[Timing_events_Num].Web_Data.repetition_Time[0], Event_cycle[Repetition]);
    else
      len += snprintf(Event_content + len, total_len - len, "\\n&nbsp;&nbsp;&nbsp;&nbsp;----- %s\\n\\n", Event_cycle[Repetition]);
    // printf("%s\r\n", Event_content);  

    sprintf(Event_str[Timing_events_Num], "Event %d : %s \\n%s\\r\\n", Timing_events_Num + 1, datetime_str,Event_content);
    heap_caps_free(Event_content);
    Timing_events_Num ++;
  }
}

void replace_special_chars(char* str) {
    char* pos;
    while ((pos = strstr(str, "&nbsp;")) != NULL) {
        memmove(pos, pos + 6, strlen(pos + 6) + 1);  // 6 = length of "&nbsp;"
        *(pos) = ' ';
    }
    while ((pos = strstr(str, "\\\\")) != NULL) {
        memmove(pos + 1, pos + 2, strlen(pos + 2) + 1);
        *(pos) = '\\';
    }
}
  
void TimerEvent_printf(Timing_RTC event){
  // Replace all "&nbsp;" with a space before printing
  char event_content_copy[sizeof(Event_str[event.Event_Number])];
  strcpy(event_content_copy, Event_str[event.Event_Number]);
  replace_special_chars(event_content_copy);

  printf("%s\r\n", event_content_copy);
}

void TimerEvent_printf_ALL(void)
{
  printf("/******************* Current RTC event *******************/ \r\n");
  for (int i = 0; i < Timing_events_Number_MAX; i++) {
    if(Events_State[i].Enable_Flag)
      TimerEvent_printf(Events_State[i]);
  }
  printf("/******************* Current RTC event *******************/\r\n\r\n ");
}
void TimerEvent_Del(Timing_RTC event){
  printf("Example Delete an RTC event%d\r\n\r\n",event.Event_Number);
  uint8_t Event_Number = event.Event_Number - 1;
  if (Events_State[Event_Number].Web_Data.SerialData) {                // 清除
    free(Events_State[Event_Number].Web_Data.SerialData);
    Events_State[Event_Number].Web_Data.SerialData = NULL;
  }
  for (int i = Event_Number; i < Timing_events_Number_MAX - 1; i++) {
    Events_State[i+1].Event_Number = Events_State[i+1].Event_Number -1;
    Events_State[i] = Events_State[i+1];  
    strcpy(Event_str[i], Event_str[i+1]);
  }  
  Events_State[Timing_events_Number_MAX - 1] = Events_State_Default;
  memset(Event_str[Timing_events_Number_MAX - 1], 0, sizeof(Event_str[Timing_events_Number_MAX - 1]));
  Timing_events_Num --;
  uint32_t Cycle_duration_GCD = calculate_repetition_gcd();
  if(Cycle_duration_GCD){
    printf("calculate repetition gcd:%ld\r\n",Cycle_duration_GCD);
    Cycle_duration = Cycle_duration_GCD; 
  }
}
void TimerEvent_Del_Number(uint8_t Event_Number){
  TimerEvent_Del(Events_State[Event_Number-1]);
}