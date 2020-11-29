/*****************************************************************************
* LEDimageServer - webserver and controller application for a screen of LEDs *
* Copyright (C) 2020 Eric Newton                                             *
*                                                                            *
* LEDimageServer is free software: you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation, either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* LEDimageServer is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU General Public License for more details.                               *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with LEDimageServer.  If not, see <https://www.gnu.org/licenses/>.   *
*****************************************************************************/

#ifndef ANIMATION_CPP
#define ANIMATION_CPP

#include "globals.h"

struct AnimationInfo {
  //char name[MAX_FILE_NAME + 1] = ""; //name of the animation
  File videoData; //file containing the animation video data
} animInfo;

void anim__processRequest(AsyncWebServerRequest* request);
String anim__templateProcessor(const String& var);
void anim__setup(AsyncWebServer* server);
void anim__updateFrame();

OperatingMode AnimationOperatingMode = {
  .setup = anim__setup,
  { .prevPath = "" },
  .updateFrame = anim__updateFrame,
};

void anim__setup(AsyncWebServer* server) {
  server->on("/action/animation", anim__processRequest);
  server
    ->serveStatic("/select_animation.html", SDFS, "/web/select_animation.html")
    .setTemplateProcessor(anim__templateProcessor)
    .setCacheControl("max-age=30");
}
void anim__updateFrame() {
  if (!animInfo.videoData.available()) { //reached end of file
    animInfo.videoData.seek(0); //bring back to the beginning
  }
  animInfo.videoData.read(frameBufferCTX.Pixels, frameBufferCTX.SizePixels); //copy GRB data to data buffer
}
void anim__processRequest(AsyncWebServerRequest* request) {
  if (!request->hasParam(F("animName"))) {
    return;
  }
  char animName[MAX_FILE_NAME];
  snprintf(animName, sizeof(animName), request->getParam("animName")->value().c_str());
  char fname[MAX_FILE_NAME];
  snprintf(fname, sizeof(fname), "/data/anim/%s/meta.json", animName);
  File tmpFile = SDFS.open(fname, "r");
  StaticJsonDocument<128> metadataJSON;
  if (!tmpFile || deserializeJson(metadataJSON, tmpFile)) {
    WRITE_OUT("Failed to open/deserialize animation metadata json file.\n");
    return;
  }
  //currently not doing anything with the number of frames since I can now just go based off of the
  //data file size
  //animInfo.numFrames = metadataJSON["frames"].as<int>();
  snprintf(AnimationOperatingMode.prevPath, sizeof(AnimationOperatingMode.prevPath), "/data/anim/%s/%s", animName, metadataJSON["prevName"].as<char*>());
  tmpFile.close();
  
  snprintf(fname, sizeof(fname), "/data/anim/%s/data.grb", animName);
  animInfo.videoData = SDFS.open(fname, "r");
  if (!animInfo.videoData) {
    WRITE_OUT("Failed to open animation data file.\n");
    return;
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
