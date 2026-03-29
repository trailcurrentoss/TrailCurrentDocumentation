#include "WS_PCF85063.h"

datetime_t datetime= {0};
datetime_t Update_datetime= {0};
static uint8_t decToBcd(int val);
static int bcdToDec(uint8_t val);


void Time_printf(void *parameter) {
  while(1){
    char datetime_str[50];
    datetime_to_str(datetime_str,datetime);
    printf("Time:%s\r\n",datetime_str);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}
void PCF85063_Init(void)      // PCF85063 initialized
{
	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL;

	I2C_Write(PCF85063_ADDRESS, RTC_CTRL_1_ADDR, &Value, 1);
  I2C_Read(PCF85063_ADDRESS, RTC_CTRL_1_ADDR,  &Value, 1);
	if(Value & RTC_CTRL_1_STOP)
		printf("PCF85063 failed to be initialized.state :%d\r\n",Value);
	else
		printf("PCF85063 is running,state :%d\r\n",Value);
    
  // 
  // Update_datetime.year = 2024;
  // Update_datetime.month = 9;
  // Update_datetime.day = 20;
  // Update_datetime.dotw = 5;
  // Update_datetime.hour = 9;
  // Update_datetime.minute = 50;
  // Update_datetime.second = 0;
  // PCF85063_Set_All(Update_datetime);
  xTaskCreatePinnedToCore(
    PCF85063Task,    
    "PCF85063Task",   
    4096,                
    NULL,                 
    3,                   
    NULL,                 
    0                   
  );  
  // xTaskCreatePinnedToCore(
  //   Time_printf,    
  //   "Time_printf",   
  //   4096,                
  //   NULL,                 
  //   3,                   
  //   NULL,                 
  //   0                   
  // );
}

void PCF85063Task(void *parameter) {
  while(1){
    PCF85063_Read_Time(&datetime);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  vTaskDelete(NULL);
}

void PCF85063_Reset()  // Reset PCF85063
{
	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL|RTC_CTRL_1_SR;
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_CTRL_1_ADDR, &Value, 1);
	if(ret != ESP_OK)
		printf("PCF85063 : Reset failure\r\n");
}
void PCF85063_Set_Time(datetime_t time) // Set Time 
{
	uint8_t buf[3] = {decToBcd(time.second),
					  decToBcd(time.minute),
					  decToBcd(time.hour)};
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_SECOND_ADDR, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Time setting failure\r\n");
}
void PCF85063_Set_Date(datetime_t date) // Set Date
{
	uint8_t buf[4] = {decToBcd(date.day),
					  decToBcd(date.dotw),
					  decToBcd(date.month),
					  decToBcd(date.year - YEAR_OFFSET)};
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_DAY_ADDR, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Date setting failed\r\n");
}

void PCF85063_Set_All(datetime_t time) // Set Time And Date
{
	uint8_t buf[7] = {decToBcd(time.second),
					  decToBcd(time.minute),
					  decToBcd(time.hour),
					  decToBcd(time.day),
					  decToBcd(time.dotw),
					  decToBcd(time.month),
					  decToBcd(time.year - YEAR_OFFSET)};
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_SECOND_ADDR, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Failed to set the date and time\r\n");
}

void PCF85063_Read_Time(datetime_t *time) // Read Time And Date
{
	uint8_t buf[7] = {0};
	esp_err_t ret = I2C_Read(PCF85063_ADDRESS, RTC_SECOND_ADDR, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Time read failure\r\n");
	else{
		time->second = bcdToDec(buf[0] & 0x7F);
		time->minute = bcdToDec(buf[1] & 0x7F);
		time->hour = bcdToDec(buf[2] & 0x3F);
		time->day = bcdToDec(buf[3] & 0x3F);
		time->dotw = bcdToDec(buf[4] & 0x07);
		time->month = bcdToDec(buf[5] & 0x1F);
		time->year = bcdToDec(buf[6]) + YEAR_OFFSET;
	}
}

void PCF85063_Enable_Alarm() // Enable Alarm and Clear Alarm flag
{
	uint8_t Value = RTC_CTRL_2_DEFAULT | RTC_CTRL_2_AIE;
	Value &= ~RTC_CTRL_2_AF;
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_CTRL_2_ADDR, &Value, 1);
	if(ret != ESP_OK)
		printf("PCF85063 : Failed to enable Alarm Flag and Clear Alarm Flag \r\n");
}

uint8_t PCF85063_Get_Alarm_Flag() // Get Alarm flag
{
	uint8_t Value = 0;
	esp_err_t ret = I2C_Read(PCF85063_ADDRESS, RTC_CTRL_2_ADDR, &Value, 1);
	if(ret != ESP_OK)
		printf("PCF85063 : Failed to obtain a warning flag.\r\n");
	else
		Value &= RTC_CTRL_2_AF | RTC_CTRL_2_AIE;
	//printf("Value = 0x%x",Value);
	return Value;
}

void PCF85063_Set_Alarm(datetime_t time) // Set Alarm
{

	uint8_t buf[5] ={
		decToBcd(time.second)&(~RTC_ALARM),
		decToBcd(time.minute)&(~RTC_ALARM),
		decToBcd(time.hour)&(~RTC_ALARM),
		//decToBcd(time.day)&(~RTC_ALARM),
		//decToBcd(time.dotw)&(~RTC_ALARM)
		RTC_ALARM, 	//disalbe day
		RTC_ALARM	//disalbe weekday
	};
	esp_err_t ret = I2C_Write(PCF85063_ADDRESS, RTC_SECOND_ALARM, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Failed to set alarm flag\r\n");
}

void PCF85063_Read_Alarm(datetime_t *time) // Read Alarm
{
	uint8_t buf[5] = {0};
	esp_err_t ret = I2C_Read(PCF85063_ADDRESS, RTC_SECOND_ALARM, buf, sizeof(buf));
	if(ret != ESP_OK)
		printf("PCF85063 : Failed to read the alarm sign\r\n");
	else{
		time->second = bcdToDec(buf[0] & 0x7F);
		time->minute = bcdToDec(buf[1] & 0x7F);
		time->hour = bcdToDec(buf[2] & 0x3F);
		time->day = bcdToDec(buf[3] & 0x3F);
		time->dotw = bcdToDec(buf[4] & 0x07);
	}
}

static uint8_t decToBcd(int val) // Convert normal decimal numbers to binary coded decimal
{
	return (uint8_t)((val / 10 * 16) + (val % 10));
}
static int bcdToDec(uint8_t val) // Convert binary coded decimal to normal decimal numbers
{
	return (int)((val / 16 * 10) + (val % 16));
}
void datetime_to_str(char *datetime_str,datetime_t time)
{
	sprintf(datetime_str, "%d.%d.%d  %d:%d:%d  %s", time.year, time.month, 
			time.day, time.hour, time.minute, time.second, Week[time.dotw]);
} 