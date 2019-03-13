/*
 * arduino_pi.h
 *
 *  Created on: Nov 4, 2018
 *      Author: raghuravi
 */

#ifndef ARDUINO_PI_H_
#define ARDUINO_PI_H_
#include <Wire.h>
#include "rgb_lcd.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <ctime.h>
#define D4            4
#define OLED_RESET 4

#define SERIAL_DEBUG_PRINTS      0
#define RGB_LCD_TEST             0
#define PUSH_BUTTON              1
#define DISPLAY_TIME_IN_LOOP     0
#define DATA_FROM_PI             0
#define DISPLAY_TEMP             0
#define SUCCESS  0
#define FAIL  -1
#define BAUD_RATE    115200
#define STR_LENGTH     1024
#define LINE_BUFF_SIZE  16
#define STRING_TO_PI_BUFFER_SIZE    128
#define ROW_0         0
#define ROW_1         1

#define LED_ON        1
#define LED_OFF       0

const int lightAdc=A2;           //light sensor analog input pin A2
const int pinAdc = A1;            //sound sensor analog input pin A1
const int led_pin= D4;
const int trig_pin=12;
const int echo_pin=13;
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

int btn,led_next_state=LED_ON ;
int incomingByte = 0;   // for incoming serial data
char menu_char=0;
String timeStr;
String line1Str;
String line2Str;
struct tm currentTime;
time_t getCurrTime;
char line1Buff[LINE_BUFF_SIZE];
char line2Buff[LINE_BUFF_SIZE];
char serial_buffer[STRING_TO_PI_BUFFER_SIZE];
//char data_str_to_pi[];
char c_str="Wed Nov 28 20:29:37 2018";
using namespace std;
Ultrasonic ultrasonic(trig_pin, echo_pin);
Adafruit_SSD1306 display(OLED_RESET);
typedef struct time_struct_t{
    char curr_day[3];
    char curr_mnth[3];
    char curr_date[2];
    char curr_time[8];
    char curr_year[4];
}time_struct;
//object lcd defined for Grove kit RGB LCD Display
/*RGB LCD Display variables*/
rgb_lcd lcd;
const int colorR = 127;
const int colorG = 127;
const int colorB = 127;

const int button = 2;       // connect a button
const int buzzer = 3;       // connect a buzzer



typedef enum WIFI_State
{
  ENABLE_WIFI=0,
  DISABLE_WIFI=-1
};


typedef enum menu{
  DISPLAY_TIME='t',
  GET_TEMPARATURE='T',
  GET_SOUND_SENSOR_DATA='s',
  GET_LIGHT_SENSOR_DATA='l',
  GET_ULTRASONIC_DATA='U',
  SEND_SENSOR_DATA_TO_PI='D',
  LCD_TEXT_DISPLAY='l',
  LED_DISPLAY='e',
  ENABLE_WIFI_MODULE,
  SEND_DATA_OVER_WIFI,
  GET_PUSH_BUTTON_DATA,
  TRIGGER_BUZZER,
  ENABLE_RELAY,
  ROTARY_ANGLE_SENSOR,
  TOUCH_SENSOR,
  ENABLE_LED
};


#endif /* ARDUINO_PI_H_ */
