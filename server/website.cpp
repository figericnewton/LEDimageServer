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

#ifndef WEBSITE_CPP
#define WEBSITE_CPP

#include "website.h"
#include "credentials.h" //defines my network's SSID and PSK, sorry nefarious users but I can't make that public.

const char* ssid = STASSID;
const char* password = STAPSK;

AsyncWebServer server(80);
String homeTemplateProcessor(const String& var) {
  WRITE_OUT("template processor called to replace: %s\n", var.c_str());
  if (var == F("DISPLAY_MODE_TEXT")) {
    //FIXME: consider creating text for each different display mode instead of this generic text
    return F("Preview of the LED display:");
  } else if ( var == "IMAGE_PREVIEW_PATH" ) {
    return String(CurrentOperatingMode->prevPath);
  }
  return String();
}
AsyncWebServer* setupWebInterface() {
  WRITE_OUT("Connecting to %s", ssid);
  WiFi.begin(ssid, password); //connects to the wifi (hopefully)
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    WRITE_OUT(".");
  }
  WRITE_OUT("\nWiFi connected\nIP address: %s\n", WiFi.localIP().toString().c_str());

  server.rewrite("/", "/home.html");
  setupAllOperatingModes(&server);
  server
    .serveStatic("/home.html", SDFS, "/web/home.html")
    .setTemplateProcessor(homeTemplateProcessor);
  server
    .serveStatic("/", SDFS, "/")
    .setCacheControl("max-age=600");
  
  //start the actual web server
  server.begin();
  WRITE_OUT("HTTP Server Started\n");
  return &server;
}

#endif
