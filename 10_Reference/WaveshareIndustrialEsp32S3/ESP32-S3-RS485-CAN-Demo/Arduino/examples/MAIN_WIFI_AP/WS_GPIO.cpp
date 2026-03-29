#include "WS_GPIO.h"

/*************************************************************  I/O Init  *************************************************************/
void digitalToggle(int pin)
{
  digitalWrite(pin, !digitalRead(pin));                                         // Toggle the state of the pin
}

