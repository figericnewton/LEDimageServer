
#ifndef DISPLAY_OFF_CPP
#define DISPLAY_OFF_CPP

#include "globals.h"

bool frameUpdated;
const RgbColor offColor(0);

void processRequest(AsyncWebServerRequest* request) {
  CurrentOperatingMode = OffOperatingMode;
  frameUpdated = false;
  request->redirect("/home.html");
  WRITE_OUT("Turning display off!");
}
void setup(AsyncWebServer* server) {
  server->on("/action/off", processRequest);
}
char* getPreviewPath() {
  return "/web/res/displayOff.png";
}
void updateFrame(uint8_t* currentFrameBuffer, NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>>* neoPixFrameBuffer) {
  if (!frameUpdated) { //only do this once
    neoPixFrameBuffer->ClearTo(offColor);
    frameUpdated = true;
  }
}
OperatingMode OffOperatingMode = {
  .setup = setup,
  .getPreviewPath = getPreviewPath,
  .updateFrame = updateFrame,
};

#endif
