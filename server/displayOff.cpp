
#ifndef DISPLAY_OFF_CPP
#define DISPLAY_OFF_CPP

#include "globals.h"

bool frameUpdated;
const RgbColor offColor(0);
void off__processRequest(AsyncWebServerRequest* request);
void off__setup(AsyncWebServer* server) {
  server->on("/action/off", off__processRequest);
}
char* off__getPreviewPath() {
  return "/web/res/displayOff.png";
}
void off__updateFrame(uint8_t* currentFrameBuffer, NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>>* neoPixFrameBuffer) {
  if (!frameUpdated) { //only do this once
    neoPixFrameBuffer->ClearTo(offColor);
    frameUpdated = true;
  }
}
OperatingMode OffOperatingMode = {
  .setup = off__setup,
  .getPreviewPath = off__getPreviewPath,
  .updateFrame = off__updateFrame,
};
void off__processRequest(AsyncWebServerRequest* request) {
  CurrentOperatingMode = OffOperatingMode;
  frameUpdated = false;
  request->redirect("/home.html");
  WRITE_OUT("Turning display off!");
}

#endif
