
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <SDFS.h>
#include <NeoPixelBus.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define PANEL_WIDTH 72
#define PANEL_HEIGHT 36
#define BYTES_PER_PIX 3

//SPIFFS and LittleFS limit to 31 characters in a file name (32 including \0)
#define MAX_FILE_NAME 31

extern NeoBuffer<NeoBufferMethod<NeoGrbFeature>> neoPixFrameBuffer;
extern NeoBufferContext<NeoGrbFeature> frameBufferCTX;

#include "debugging.h"
#include "operating_mode.h"

#endif
