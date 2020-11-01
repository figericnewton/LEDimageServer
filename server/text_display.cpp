
#ifndef TEXT_DISPLAY_CPP
#define TEXT_DISPLAY_CPP
/*
#include "globals.h"
#include "font.h"

#define MAX_TEXT_LENGTH 100

class TextOperatingMode: public OperatingModeHandler {
  char textDisplay[MAX_TEXT_LENGTH + 1];
  bool scrollText;
  uint16_t scrollOffset;
  public:
    void setup(AsyncWebServer* server) {
      server->on("/action/text", processRequest);
      server->serveStatic("/text_display.html", SDFS, "/web/text_display.html");
    }
    char* getPreviewPath() {
      return F("/web/res/placeholder.png");
    }
    void updateFrame() {
      //FIXME: put in logic for text display
      memset(currentFrame, 0, sizeof(currentFrame));
    }
  private:
    void processRequest(AsyncWebServerRequest* request) {
      if (!request->hasParam("textInput") || !request->hasParam("scrollText")) {
        return;
      }
      snprintf(textDisplay, sizeof(textDisplay), request->getParam("textInput")->value());
      if (request->getParam("scrollText")->value() == "true") {
        scrollText = true;
      } else {
        scrollText = false;
      }
      scrollOffset = 0;
      currentMode = &this;
      //FIXME: finish any additional setup that's necessary
      request->redirect("/home.html");
    }
};
ImageOperatingMode textOpMode;
*/
#endif
