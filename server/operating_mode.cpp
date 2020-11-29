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

#ifndef OPERATING_MODE_CPP
#define OPERATING_MODE_CPP

#include "globals.h"

OperatingMode *CurrentOperatingMode;
extern OperatingMode OffOperatingMode;
extern OperatingMode AnimationOperatingMode;
extern OperatingMode PongOperatingMode;
extern OperatingMode StreamOperatingMode;

void setupAllOperatingModes(AsyncWebServer* server) {
  OffOperatingMode.setup(server);
  AnimationOperatingMode.setup(server);
  PongOperatingMode.setup(server);
  StreamOperatingMode.setup(server);
  CurrentOperatingMode = &OffOperatingMode; //set default
}

#endif
