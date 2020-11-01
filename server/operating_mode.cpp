
#ifndef OPERATING_MODE_CPP
#define OPERATING_MODE_CPP

#include "globals.h"

OperatingMode CurrentOperatingMode;
extern OperatingMode OffOperatingMode;
extern OperatingMode AnimationOperatingMode;
//FIXME: add other modes here

void setupAllOperatingModes(AsyncWebServer* server) {
  OffOperatingMode.setup(server);
  AnimationOperatingMode.setup(server);
  //FIXME: add other modes here
  CurrentOperatingMode = OffOperatingMode; //set default
}

#endif
