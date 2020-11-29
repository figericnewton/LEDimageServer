/*****************************************************************************
* LEDimageServer - webserver and controller application for a screen of LEDs *
* Copyright (C) 2020 Eric Newton                                             *
*                                                                            *
* LEDimageServer is free software: you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation, either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* LEDimageServer is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU General Public License for more details.                               *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with LEDimageServer.  If not, see <https://www.gnu.org/licenses/>.   *
*****************************************************************************/

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
