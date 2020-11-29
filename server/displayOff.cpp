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

#ifndef DISPLAY_OFF_CPP
#define DISPLAY_OFF_CPP

#include "globals.h"

const RgbColor offColor(0);
void off__setup(AsyncWebServer* server);
void off__processRequest(AsyncWebServerRequest* request);
void off__updateFrame() {
  neoPixFrameBuffer.ClearTo(offColor);
}
OperatingMode OffOperatingMode = {
  .setup = off__setup,
  { .prevPath = "/web/res/displayOff.png" },
  .updateFrame = off__updateFrame,
};
void off__setup(AsyncWebServer* server) {
  server->on("/action/off", off__processRequest);
}
void off__processRequest(AsyncWebServerRequest* request) {
  CurrentOperatingMode = &OffOperatingMode;
  request->redirect("/home.html");
  WRITE_OUT("Turning display off!\n");
}

#endif
