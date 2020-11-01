
#ifndef OPERATING_MODE_CPP
#define OPERATING_MODE_CPP

#include "globals.h"

OperatingMode CurrentOperatingMode;

void setupAllOperatingModes(AsyncWebServer* server) {
  OffOperatingMode.setup(server);
  //FIXME: add other modes here
  CurrentOperatingMode = OffOperatingMode; //set default
}

#endif
