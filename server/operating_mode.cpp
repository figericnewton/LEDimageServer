
#ifndef OPERATING_MODE_CPP
#define OPERATING_MODE_CPP

#include "globals.h"

OperatingMode *CurrentOperatingMode;
extern OperatingMode OffOperatingMode;
extern OperatingMode AnimationOperatingMode;
extern OperatingMode PongOperatingMode;
extern OperatingMode VideoOperatingMode;
//FIXME: add other modes here

void setupAllOperatingModes(AsyncWebServer* server) {
  OffOperatingMode.setup(server);
  AnimationOperatingMode.setup(server);
  PongOperatingMode.setup(server);
  VideoOperatingMode.setup(server);
  //FIXME: add other modes here
  CurrentOperatingMode = &OffOperatingMode; //set default
}

#endif
