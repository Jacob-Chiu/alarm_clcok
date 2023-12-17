#include <Arduino.h>
#include <Wire.h> //I2C library
#include "bitmaps.h"

//Wifi
#include <ESP8266WiFi.h>
#include <WifiInfo.h> //this header file is located at /Arduino/libraries/WifiInfo and contains my Wifi SSID/password which I won't share with you.

//IR remote
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

//clock libraries
#include <TimeLib.h>
#include <RtcDS3231.h> //RTC library
#include <NTPClient.h>
#include <WiFiUdp.h>

//7-segment libraries
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//u8g2
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//Serialwombat
#include <SerialWombat.h>

//EEPROM
#include <EEPROM.h>
