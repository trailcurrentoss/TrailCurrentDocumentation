
#include "WS_Bluetooth.h"
#include "WS_RS485.h"
#include "WS_CAN.h"
#include "WS_RTC.h"
#include "WS_WIFI.h"


uint32_t Simulated_time=0;      // Analog time counting

/********************************************************  Initializing  ********************************************************/
void setup() { 
  I2C_Init();
  RS485_Init();
  CAN_Init();
  RTC_Init();// RTC
  WIFI_Init();// WIFI
  Bluetooth_Init();// Bluetooth
  
  printf("Connect to the WIFI network named \"ESP32-S3-RS485-CAN\" and access the Internet using the connected IP address!!!\r\n");
}

/**********************************************************  While  **********************************************************/
void loop() {

}