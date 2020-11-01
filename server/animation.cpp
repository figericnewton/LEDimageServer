
#ifndef ANIMATION_CPP
#define ANIMATION_CPP
/*
#include "globals.h"

class AnimationOperatingMode: public OperatingModeHandler {
  char animName[MAX_FILE_NAME + 1];
  char prevPath[MAX_FILE_NAME + 1];
  uint16_t numFrames;
  uint16_t frame = 1;
  public:
    void setup(AsyncWebServer* server) {
      server->on("/action/animation", processRequest);
      server
        ->serveStatic("/select_animation.html", SD, "/web/select_animation.html")
        .setTemplateProcessor(templateProcessor);
    }
    char* getPreviewPath() {
      snprintf(prevPath, sizeof(prevPath), "/data/anim/%s/pvw.gif", animName);
      return prevPath;
    }
    void updateFrame() {
      char fname[MAX_FILE_NAME + 1];
      snprintf(fname, sizeof(fname), "/data/anim/%s/%i.grb", animName, frame);

      File imgFile = SDFS.open(fname, FILE_READ); //open the image frame
      if (!imgFile) {
        WRITE_OUT("file open failed\n");
        return;
      }
      imgFile.readBytes((char *)currentFrame, sizeof(currentFrame)); //copy GRB data to data buffer
      imgFile.close();
      frame++;
      if (frame > numFrames) {
        frame = 1;
      }
    }
  private:
    void processRequest(AsyncWebServerRequest* request) {
      if (!request->hasParam("animName")) {
        return;
      }
      snprintf(animName, sizeof(animName), request->getParam("animName")->value());
      frame = 1;
      char fname[MAX_FILE_NAME];
      File dir, f;
      snprintf(fname, sizeof(fname), "/data/anim/%s", animName);
      dir = SDFS.open(fname);
      numFrames = 0;
      while (f = dir.openNextFile()) {
        if (f.name().endsWith(".grb")) {
          numFrames++;
        }
      }
      currentMode = &this;
      request->redirect("/home.html");
    }
    String templateProcessor(const String& var) {
      String retHTML = String();
      File dir, f;
      if (var == "ANIMATION_PREVIEW_CONTENTS") {
        dir = SDFS.open(F("/data/anim"));
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
};
AnimationOperatingMode animationOpMode;
*/
#endif
