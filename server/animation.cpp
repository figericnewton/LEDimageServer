
#ifndef ANIMATION_CPP
#define ANIMATION_CPP

#include "globals.h"

struct AnimationInfo {
  char name[MAX_FILE_NAME + 1] = ""; //name of the animation
  uint16_t numFrames; //total number of frames
  uint16_t frame; //current frame number
} animInfo;

void anim__processRequest(AsyncWebServerRequest* request);
String anim__templateProcessor(const String& var);

void anim__setup(AsyncWebServer* server) {
  server->on("/action/animation", anim__processRequest);
  server
    ->serveStatic("/select_animation.html", SDFS, "/web/select_animation.html")
    .setTemplateProcessor(anim__templateProcessor)
    .setCacheControl("max-age=30");
}
void anim__updateFrame(uint8_t* currentFrameBuffer, NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>> neoPixFrameBuffer) {
  //WRITE_OUT("anim__updateFrame\n");
  char fname[MAX_FILE_NAME + 1];
  snprintf(fname, sizeof(fname), "/data/anim/%s/%i.grb", animInfo.name, animInfo.frame);

  animInfo.frame++;
  if (animInfo.frame > animInfo.numFrames) {
    animInfo.frame = 1;
  }

  WRITE_OUT("Opening ");
  WRITE_OUT(fname);
  File imgFile = SDFS.open(fname, "r"); //open the image frame
  if (imgFile) {
    WRITE_OUT(" -- success\n");
  } else {
    WRITE_OUT(" -- FAILURE\n");
    return;
  }
  //There's some sort of issue going on when the website asynchronously tries to access the SD card
  //at the same time as I'm accessing it. For now I've found that a work-around is to break down
  //reading the file into smaller chunks. In the future if the issue is fixed in the (SD card?) library
  //then it is better for performance to do a single call to readBytes.
  //it appears that this may be a memory issue after all -- need to optimize usage!
  //imgFile.read(currentFrameBuffer, BYTES_PER_PIX * PANEL_WIDTH * PANEL_HEIGHT); //copy GRB data to data buffer
  uint8_t* ptrRead = currentFrameBuffer;
  while (imgFile.available()) {
    ptrRead += imgFile.read(ptrRead, 512); //read in standard block sizes
  }
  imgFile.close();
}
OperatingMode AnimationOperatingMode = {
  .setup = anim__setup,
  { .prevPath = "" },
  .updateFrame = anim__updateFrame,
};
void anim__processRequest(AsyncWebServerRequest* request) {
  if (!request->hasParam(F("animName"))) {
    return;
  }
  snprintf(animInfo.name, sizeof(animInfo.name), request->getParam("animName")->value().c_str());
  animInfo.frame = 1;
  char fname[MAX_FILE_NAME];
  snprintf(fname, sizeof(fname), "/data/anim/%s/meta.txt", animInfo.name);
  File metadata = SDFS.open(fname, "r");
  char tmpRead[20]; //hardcode to 20bytes for now, if more is needed in future will have to update
  long bytesRead;
  while (metadata.available() && ((bytesRead = metadata.readBytesUntil(',', tmpRead, sizeof(tmpRead) - 1)) > 0)) {
    tmpRead[bytesRead] = '\0'; //add null terminator
    if (strcmp(tmpRead, "frames") == 0) {
      bytesRead = metadata.readBytesUntil('\n', tmpRead, sizeof(tmpRead) - 1);
      tmpRead[bytesRead] = '\0'; //add null terminator
      animInfo.numFrames = atoi(tmpRead);
    } else if (strcmp(tmpRead, "prevName") == 0) {
      bytesRead = metadata.readBytesUntil('\n', tmpRead, sizeof(tmpRead) - 1);
      tmpRead[bytesRead] = '\0'; //add null terminator
      snprintf(AnimationOperatingMode.prevPath, sizeof(AnimationOperatingMode.prevPath), "/data/anim/%s/%s", animInfo.name, tmpRead);
    }
  }
  CurrentOperatingMode = &AnimationOperatingMode;
  WRITE_OUT("Displaying animation!\n");
  request->redirect("/home.html");
}
String anim__templateProcessor(const String& var) {
  String retHTML = String();
  Dir dir;
  if (var == F("ANIMATION_PREVIEW_CONTENTS")) {
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
