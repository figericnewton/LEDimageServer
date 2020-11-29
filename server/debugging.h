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

#ifndef DEBUGGING_H
#define DEBUGGING_H

//uncomment as desired when debugging
#define DEBUG
//#define DEBUG_TIMER_ENABLED
//#define DEBUG_MEMORY_ENABLED

#ifdef DEBUG
  #define WRITE_OUT(...) Serial.printf(__VA_ARGS__)
#else
  #define WRITE_OUT(...)  
#endif

#ifdef DEBUG_TIMER_ENABLED
  void startTimer();
  void stopTimer();
#else
  #define startTimer(x)  
  #define stopTimer(x)  
#endif

#ifdef DEBUG_MEMORY_ENABLED
  void echoFreeMemory();
#else
  #define echoFreeMemory(x)   
#endif

#endif
