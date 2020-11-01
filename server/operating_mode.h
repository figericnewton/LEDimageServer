
#ifndef OPERATING_MODE_H
#define OPERATING_MODE_H

#include "globals.h"

// interface for LED operating mode objects
typedef struct OperatingMode_t {
  //sets up this operation mode's part of the website
  void (*setup)(AsyncWebServer* server);

  //returns a path to the preview image for this operating mode
  char* (*getPreviewPath)();

  //updates the frame and then returns a pointer to the neo buffer
  void (*updateFrame)(uint8_t* currentFrameBuffer, NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>>* neoPixFrameBuffer);
} OperatingMode;

extern OperatingMode CurrentOperatingMode;
extern OperatingMode OffOperatingMode;
//FIXME: add other modes here

void setupAllOperatingModes(AsyncWebServer* server);
//void setCurrentOperatingMode(OperatingMode newMode);

#endif
