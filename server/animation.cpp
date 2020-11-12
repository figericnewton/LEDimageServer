
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
  { .prevPath = "" },
  .updateFrame = anim__updateFrame,
};
void anim__processRequest(AsyncWebServerRequest* request) {
  if (!request->hasParam("animName")) {
    return;
  }
  snprintf(animName, sizeof(animName), request->getParam("animName")->value().c_str());
  snprintf(AnimationOperatingMode.prevPath, sizeof(AnimationOperatingMode.prevPath), "/data/anim/%s/pvw.gif", animName);
  frame = 1;
  char fname[MAX_FILE_NAME];
  Dir dir;
  snprintf(fname, sizeof(fname), "/data/anim/%s", animName);
  dir = SDFS.openDir(fname);
  numFrames = -1; // FIXME: rework this to create a metafile that contains how many frames there are and any other information we want
  while (dir.next()) {
    //if (f.name().endsWith(".grb")) {
      numFrames++;
    //}
  }
  CurrentOperatingMode = &AnimationOperatingMode;
  WRITE_OUT("Displaying animation!\n");
  request->redirect("/home.html");
}
String anim__templateProcessor(const String& var) {
  String retHTML = String();
  Dir dir;
  if (var == "ANIMATION_PREVIEW_CONTENTS") {
    dir = SDFS.openDir(F("/data/anim"));
    while (dir.next()) {
      retHTML += F("<a href=\"/action/animation?animName=");
      retHTML += dir.fileName();
      retHTML += F("\"><img src=\"/data/anim/");
      retHTML += dir.fileName();
      retHTML += F("/pvw.gif\" style=\"width:100%%\" alt=\"Animation Preview\"></a>\n");
    }
  }
  return retHTML;
}

#endif
