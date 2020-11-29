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

#ifndef DEBUGGING_CPP
#define DEBUGGING_CPP

#include "globals.h"

#ifdef DEBUG_TIMER_ENABLED
long lastTime = 0;
void startTimer() {
  lastTime = millis();
}
void stopTimer() {
  long curTime = millis();
  WRITE_OUT("timer: %lu millis\n", curTime - lastTime);
  lastTime = curTime;
}
#endif

#ifdef DEBUG_MEMORY_ENABLED
void echoFreeMemory() {
  void *stackptr;
  void *heapptr = (uint8_t *)malloc(4);
  WRITE_OUT("memory left: %lu\n", &stackptr - heapptr);
  free(stackptr);
}
#endif
#endif
