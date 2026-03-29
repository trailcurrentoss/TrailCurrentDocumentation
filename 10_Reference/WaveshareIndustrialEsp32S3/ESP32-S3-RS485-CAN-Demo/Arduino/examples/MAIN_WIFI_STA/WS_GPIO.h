#pragma once

#include <Arduino.h>     // Reference the ESP32 built-in serial port library

/*************************************************************  I/O  *************************************************************/
#define GPIO_PIN_CH1      47    // CH1 Control GPIO

#define TXD1              17    // The TXD of UART corresponds to GPIO   RS485
#define RXD1              18    //The RXD of UART1 corresponds to GPIO   RS485
#define TXD1EN            21

#define TXD2              15    // The TXD of TWAI corresponds to GPIO   CAN
#define RXD2              16    // The RXD of TWAI corresponds to GPIO   CAN



/*************************************************************  I/O  *************************************************************/
void digitalToggle(int pin);
