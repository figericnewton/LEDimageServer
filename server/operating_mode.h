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

#ifndef OPERATING_MODE_H
#define OPERATING_MODE_H

#include "globals.h"

// interface for LED operating mode objects
typedef struct OperatingMode_t {
  //sets up this operation mode's part of the website
  void (*setup)(AsyncWebServer* server);

  char prevPath[MAX_FILE_NAME + 1];

  void (*updateFrame)();
} OperatingMode;

extern OperatingMode *CurrentOperatingMode;

void setupAllOperatingModes(AsyncWebServer* server);

#endif
