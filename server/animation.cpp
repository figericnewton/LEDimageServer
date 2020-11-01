
#ifndef ANIMATION_CPP
#define ANIMATION_CPP

#include "globals.h"

char animName[MAX_FILE_NAME + 1];
uint16_t numFrames;
uint16_t frame = 1;

void anim__processRequest(AsyncWebServerRequest* request);
String anim__templateProcessor(const String& var);

void anim__setup(AsyncWebServer* server) {
  server->on("/action/animation", anim__processRequest);
  server
    ->serveStatic("/select_animation.html", SDFS, "/web/select_animation.html")
    .setTemplateProcessor(anim__templateProcessor);
}
char* anim__getPreviewPath() {
  char prevPath[MAX_FILE_NAME + 1];
  snprintf(prevPath, sizeof(prevPath), "/data/anim/%s/pvw.gif", animName);
  return prevPath;
}
void anim__updateFrame(uint8_t* currentFrameBuffer, NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>>* neoPixFrameBuffer) {
  char fname[MAX_FILE_NAME + 1];
  snprintf(fname, sizeof(fname), "/data/anim/%s/%i.grb", animName, frame);

  File imgFile = SDFS.open(fname, "r"); //open the image frame
  if (!imgFile) {
    WRITE_OUT("file open failed\n");
    return;
  }
  imgFile.readBytes((char *)currentFrameBuffer, sizeof(currentFrameBuffer)); //copy GRB data to data buffer
  imgFile.close();
  frame++;
  if (frame > numFrames) {
    frame = 1;
  }
}
OperatingMode AnimationOperatingMode = {
  .setup = anim__setup,
  .getPreviewPath = anim__getPreviewPath,
  .updateFrame = anim__updateFrame,
};
void anim__processRequest(AsyncWebServerRequest* request) {
  if (!request->hasParam("animName")) {
    return;
  }
  snprintf(animName, sizeof(animName), request->getParam("animName")->value().c_str());
  frame = 1;
  char fname[MAX_FILE_NAME];
  File dir, f;
  snprintf(fname, sizeof(fname), "/data/anim/%s", animName);
  dir = SDFS.open(fname, "r");
  numFrames = -1; // FIXME: rework this to create a metafile that contains how many frames there are and any other information we want
  while (f = dir.openNextFile()) {
    //if (f.name().endsWith(".grb")) {
      numFrames++;
    //}
  }
  CurrentOperatingMode = AnimationOperatingMode;
  WRITE_OUT("Displaying animation!\n");
  request->redirect("/home.html");
}
String anim__templateProcessor(const String& var) {
  String retHTML = String();
  File dir, f;
  if (var == "ANIMATION_PREVIEW_CONTENTS") {
    dir = SDFS.open(F("/data/anim"), "r");
    while (f = dir.openNextFile()) {
      retHTML += F("<a href=\"/action/animation?animName=");
      retHTML += f.name();
      retHTML += F("\"><img src=\"/data/anim/");
      retHTML += f.name();
      retHTML += F("/pvw.gif\" style=\"width:100%%\" alt=\"Animation Preview\"></a>\n");
    }
  }
  return retHTML;
}

#endif
