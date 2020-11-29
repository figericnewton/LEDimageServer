
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
