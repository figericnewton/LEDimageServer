
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <SDFS.h>
#include <NeoPixelBus.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//uncomment when debugging
#define DEBUG
#ifdef DEBUG
  #define WRITE_OUT(x) Serial.print(x)
#else
  #define WRITE_OUT(x)  
#endif

#define PANEL_WIDTH 75
#define PANEL_HEIGHT 36
#define BYTES_PER_PIX 3

//SPIFFS and LittleFS limit to 31 characters in a file name (32 including \0)
#define MAX_FILE_NAME 31

#include "operating_mode.h"

#endif
