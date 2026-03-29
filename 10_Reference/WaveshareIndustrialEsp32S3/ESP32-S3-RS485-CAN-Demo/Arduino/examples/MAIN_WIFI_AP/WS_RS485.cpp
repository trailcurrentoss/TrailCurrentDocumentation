#include "WS_RS485.h"
#include <algorithm>

HardwareSerial lidarSerial(1);  // Using serial port 1

uint8_t buf[1000] = {0};          // Data storage area
double  transmission_time = 0;
double RS485_cmd_Time = 0;
char * RS485_Read_Data;
size_t RS485_Received_Len;
uint8_t RS485_Read_Data_Type = 0;        
unsigned long RS485_BaudRate = 9600;
void RS485_Init()                                             // Initializing serial port
{    
  RS485_BaudRate = 9600;                                            // Set the baud rate of the serial port                                              
  lidarSerial.begin(RS485_BaudRate, SERIAL_8N1, RXD1, TXD1);        // Initializing serial port
  if (!lidarSerial.setPins(-1, -1, -1, TXD1EN)) {
    printf("Failed to set TXDEN pins\r\n");
  }
  if (!lidarSerial.setMode(UART_MODE_RS485_HALF_DUPLEX)) {
    printf("Failed to set RS485 mode\r\n");
  }
  transmission_time = 10.0 / RS485_BaudRate * 1000 ;
  RS485_cmd_Time = transmission_time*8;                       // 8:data length
  xTaskCreatePinnedToCore(
    RS485Task,    
    "RS485Task",   
    4096,                
    NULL,                 
    3,                   
    NULL,                 
    0                   
  );
}

void RS485_UpdateBaudRate(unsigned long baud)                                             // Initializing serial port
{    
  printf("Update the RS485 baud rate to:%ld\r\n",baud);
  lidarSerial.updateBaudRate(baud);
}
void SetData(uint8_t* data, size_t length) {
  lidarSerial.write(data, length);                          // Send data from the RS485
}
void ReadData(uint8_t* buf, uint8_t length) {
  uint8_t Receive_Flag = 0;       
  Receive_Flag = lidarSerial.available();
  if (Receive_Flag >= length) {
    lidarSerial.readBytes(buf, length); 
    char printBuf[length * 3 + 1];
    sprintf(printBuf, "Received data: ");
    for (int i = 0; i < length; i++) {
      sprintf(printBuf + strlen(printBuf), "%02X ", buf[i]); 
    }
    printf(printBuf); 
    /*************************
    Add a receiving data handler
    *************************/
    Receive_Flag = 0;
    memset(buf, 0, sizeof(buf));   
  }
}
void RS485Task(void *parameter) {
  RS485_Read_Data = (char *)heap_caps_malloc(RS485_Received_Len_MAX, MALLOC_CAP_SPIRAM);
  memset(RS485_Read_Data,0, RS485_Received_Len_MAX);
  while(1){
    RS485_Loop();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelete(NULL);
}

void RS485_Loop()
{
  uint8_t Receive_Flag = 0;       // Receiving mark
  Receive_Flag = lidarSerial.available();    

  if (Receive_Flag > 0) {
    if(RS485_cmd_Time > 1)              // Time greater than 1 millisecond
      delay((uint16_t)RS485_cmd_Time);
    else                      // Time is less than 1 millisecond 
      delay(1);
    Receive_Flag = lidarSerial.available();
    lidarSerial.readBytes(buf, Receive_Flag);              // The Receive_Flag length is read
    
    printf("RS485 Read Data: ");
    if(RS485_Read_Data_Type){
      for (int i = 0; i < Receive_Flag; i++) {
        printf("0x%02x ", buf[i]);
      }
    }
    else{
      for (int i = 0; i < Receive_Flag; i++) {
        if (buf[i] >= 32 && buf[i] <= 126) {  // 检查是否是可打印字符
          printf("%c", buf[i]);  // 打印可打印字符
        } else {
          printf(" ");  // 对不可打印字符，打印"."代替
        }
      }
    }
    printf("\r\n");

    
    char datetime_str[50];
    datetime_to_str(datetime_str,datetime);
    if(RS485_Received_Len + sizeof(datetime_str) + Receive_Flag + 3 >= RS485_Received_Len_MAX)
    {
      printf("Note : The data received by RS485 is full.\r\n");
      return;
    }    
    if(RS485_Read_Data_Type){
      char temp[Receive_Flag * 6 + 1] = {0};
      for (int i = 0; i < Receive_Flag; i++) {
        char hex_byte[6];
        snprintf(hex_byte, sizeof(hex_byte), "0x%02X ", buf[i]);
        strcat(temp, hex_byte);
      }
      RS485_Received_Len += snprintf(RS485_Read_Data + RS485_Received_Len, RS485_Received_Len_MAX - RS485_Received_Len, "%s :%s\n", datetime_str, temp);
    }
    else{
      char temp[Receive_Flag + 1] = {0};
      memcpy(temp, buf, Receive_Flag);
      temp[Receive_Flag] = '\0';
      RS485_Received_Len += snprintf(RS485_Read_Data + RS485_Received_Len, RS485_Received_Len_MAX - RS485_Received_Len, "%s :%s\n", datetime_str, temp);
    }
    
    Receive_Flag=0;
    memset(buf,0, sizeof(buf));
  }
}