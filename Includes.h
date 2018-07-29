#ifndef INCLUDES
#define INCLUDES

//external libraries
#include <String.h>
#include <Wire.h> //used in PressureSensor.ino, for I2C comms to sensor
#include <SPI.h>  //used in DataLogging.ino, for SPI comms to SD
#include <SD.h>   //used in DataLogging.ino, for SD commands
#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

//internal headers
#include "FunctionPrototypes.h"
#include "Settings.h"

#endif
