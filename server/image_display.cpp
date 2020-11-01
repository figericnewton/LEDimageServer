
#ifndef IMAGE_DISPLAY_CPP
#define IMAGE_DISPLAY_CPP
/*
#include "globals.h"

class ImageOperatingMode: public OperatingModeHandler {
  char imgName[MAX_FILE_NAME + 1];
  bool scrollImg;
  uint16_t imageScrollOffset;
  public:
    void setup(AsyncWebServer* server) {
      server->on("/action/image", processRequest);
      server
        ->serveStatic("/select_image.html", SDFS, "/web/select_image.html")
        .setTemplateProcessor(templateProcessor);
      //FIXME: include means for handling image upload to the server
    }
    char* getPreviewPath() {
      char prevPath[MAX_FILE_NAME + 1];
      snprintf(prevPath, sizeof(prevPath), "/data/img/%s.bmp", imgName);
      return prevPath;
    }
    void updateFrame() {
      char fname[MAX_FILE_NAME + 1];
      snprintf(fname, sizeof(fname), "/data/img/%s.grb", imgName);

      File imgFile = SDFS.open(fname, FILE_READ); //open the image frame
      if (!imgFile) {
        WRITE_OUT(F("Failed to open: "));
        WRITE_OUT(fname);
        WRITE_OUT("\n");
        return;
      }
      uint8_t *writePosition;
      for (int row = 0; row < PANEL_HEIGHT; row++) {
        writePosition = &currentFrame[row*BYTES_PER_PIX*PANEL_WIDTH + (PANEL_WIDTH - imageScrollOffset)*BYTES_PER_PIX];
        imgFile.readBytes((char *)writePosition, BYTES_PER_PIX*imageScrollOffset);
        writePosition = &currentFrame[row*BYTES_PER_PIX*PANEL_WIDTH];
        imgFile.readBytes((char *)writePosition, BYTES_PER_PIX*(PANEL_WIDTH - imageScrollOffset));
      }
      imgFile.close();
      if (scrollImg) {
        imageScrollOffset++;
        if (imageScrollOffset == PANEL_WIDTH) {
          imageScrollOffset = 0;
        }
      }
    }
  private:
    void processRequest(AsyncWebServerRequest* request) {
      if (!request->hasParam("imgName") || !request->hasParam("scrollImg")) {
        return;
      }
      snprintf(imgFileName, sizeof(imgFileName), request->getParam("imgName")->value());
      if (request->getParam("scrollImg")->value() == "true") {
        scrollImg = true;
      } else {
        scrollImg = false;
      }
      imageScrollOffset = 0;
      currentMode = &this;
      request->redirect("/home.html");
    }
    String templateProcessor(const String& var) {
      String retHTML = String();
      int indexExt;
      String fname, grbFname;
      File dir, f;
      if (var == "ANIMATION_PREVIEW_CONTENTS") {
        dir = SDFS.open(F("/data/img"));
        while (f = dir.openNextFile()) {
          fname = f.name();
          indexExt = fname.lastIndexOf('.');
          if (indexExt < 0 || fname.substring(indexExt + 1) == "grb") {
            continue;
          }
          grbFname = fname.substring(0, indexExt) + ".grb";
          
          retHTML += F("<a href=\"/action/image?imgName=");
          retHTML += grbFname;
          retHTML += F("\"><img src=\"/data/img/");
          retHTML += fname;
          retHTML += F("\" style=\"width:100%%\" alt=\"Image Preview\"></a>\n");
        }
      }
      return retHTML;
    }
};
ImageOperatingMode imageOpMode;
*/
#endif
