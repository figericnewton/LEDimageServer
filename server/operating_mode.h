
#ifndef OPERATING_MODE_H
#define OPERATING_MODE_H

#include "globals.h"

// interface for LED operating mode objects
typedef struct OperatingMode_t {
  //sets up this operation mode's part of the website
  void (*setup)(AsyncWebServer* server);

  char prevPath[MAX_FILE_NAME + 1];

  //updates the frame and then returns a pointer to the neo buffer
  void (*updateFrame)(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> *neoPixFrameBuffer);
} OperatingMode;

extern OperatingMode *CurrentOperatingMode;

void setupAllOperatingModes(AsyncWebServer* server);
//void setCurrentOperatingMode(OperatingMode newMode);

#endif
