
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
