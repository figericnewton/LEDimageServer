
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h" //defines my network's SSID and PSK, sorry nefarious users but I can't make that public.

#define MAX_HTML_LINE 500

const char* ssid = STASSID;
const char* password = STAPSK;

File tmpUpFile; // a File object for reading in the file being transmitted
ESP8266WebServer server(80);
void replaceIfFoundInBuffer(char* bufStr, const char* srcStr, const char* repStr) {
  char* chPtr = bufStr;
  char* chPtr2;
  int lookahead, srcLen, repLen;
  while (*chPtr) {
    lookahead = 0;
    while ( chPtr[lookahead] && srcStr[lookahead] && (chPtr[lookahead] == srcStr[lookahead]) ) { lookahead++; }
    if (!srcStr[lookahead]) { //found a match for the replacement
      srcLen = strlen(srcStr);
      repLen = strlen(repStr);
      memmove((void*)&chPtr[repLen], (void*)&chPtr[srcLen], strlen(&chPtr[srcLen]) + 1); //shift buffer to make right amount of space for repStr
      memcpy((void*)chPtr, (void*)repStr, strlen(repStr)); //copy repStr into the gap
      return;
    }
    chPtr++;
  }
}
void streamFile(const char* htmlFileName) {
  File htmlFile = SD.open(htmlFileName, FILE_READ);
  if (!htmlFile) {
    WRITE_OUT("Failed to open ");
    WRITE_OUT(htmlFileName);
    WRITE_OUT("\n");
  }
  server.streamFile(htmlFile, "text/html");
  htmlFile.close();
}
void streamFileWithReplacements(const char* htmlFileName, const char **srcStrs, const char **repStrs, const uint8_t nReps) {
  char lineBuffer[MAX_HTML_LINE];
  int bytesRead;
  File htmlFile = SD.open(htmlFileName, FILE_READ);
  if (!htmlFile) {
    WRITE_OUT("Failed to open ");
    WRITE_OUT(htmlFileName);
    WRITE_OUT("\n");
    server.send(404, "text/plain", "404: Not Found");
    return;
  }
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  while (htmlFile.available()) {
    bytesRead = htmlFile.readBytesUntil('\n', lineBuffer, MAX_HTML_LINE);
    if (!bytesRead) { continue; } //skip empty lines to avoid chunked content being terminated early
    lineBuffer[bytesRead] = '\n'; //add back in the newline
    lineBuffer[bytesRead+1] = '\0'; //add string terminator
    for (uint8_t i = 0; i < nReps; i++) {
      replaceIfFoundInBuffer(lineBuffer, srcStrs[i], repStrs[i]);
    }
    server.sendContent(lineBuffer);
  }
  htmlFile.close();
  server.sendContent(""); //chunked content must be ended by sending an additional empty line
  server.client().stop();
}
void sendHomePage() {
  const char *srcStrs[] = { "DISPLAY_MODE_TEXT", "IMAGE_PREVIEW_PATH", "SINGLE_IMAGE_DISPLAY", "FIVE_IMAGE_DISPLAY",
                       "IMAGE_0", "IMAGE_1", "IMAGE_2", "IMAGE_3", "IMAGE_4" };
  char *repStrs[9];
  char displayModeText[200];              repStrs[0] = displayModeText;
  char imagePreviewPath[MAX_FILE_NAME+1]; repStrs[1] = imagePreviewPath;
  char singleImageDisplay[10];            repStrs[2] = singleImageDisplay;
  char fiveImageDisplay[10];              repStrs[3] = fiveImageDisplay;
  char image0[MAX_FILE_NAME+1];           repStrs[4] = image0;
  char image1[MAX_FILE_NAME+1];           repStrs[5] = image1;
  char image2[MAX_FILE_NAME+1];           repStrs[6] = image2;
  char image3[MAX_FILE_NAME+1];           repStrs[7] = image3;
  char image4[MAX_FILE_NAME+1];           repStrs[8] = image4;
  
  if (displayMode == DISP_TEXT) {
    snprintf(displayModeText, sizeof(displayModeText), "Server is in text display mode, displaying the message:\n%s", textDisplay);
    snprintf(imagePreviewPath, sizeof(imagePreviewPath), "/web/res/placeholder.png");
    snprintf(singleImageDisplay, sizeof(singleImageDisplay), "none");
    snprintf(fiveImageDisplay, sizeof(fiveImageDisplay), "default");
    for (uint8_t i = 0; i < 5; i++) {
      snprintf(repStrs[4 + i], MAX_FILE_NAME+1, "/data/font/%c.bmp", textDisplay[i]);
    }
  }
  else { //not in text display mode
    snprintf(singleImageDisplay, sizeof(singleImageDisplay), "default");
    snprintf(fiveImageDisplay, sizeof(fiveImageDisplay), "none");
    for (uint8_t i = 0; i < 5; i++) {
      snprintf(repStrs[4 + i], MAX_FILE_NAME+1, "/web/res/placeholder.png");
    }
    switch (displayMode) {
      case DISP_OFF:
        snprintf(displayModeText, sizeof(displayModeText), "Display is currently turned off! Select an option from the side pane to setup what you want to display.");
        snprintf(imagePreviewPath, sizeof(imagePreviewPath), "/web/res/displayOff.png");
        break;
      case DISP_IMAGE:
        snprintf(displayModeText, sizeof(displayModeText), "Currently displaying this image:");
        snprintf(imagePreviewPath, sizeof(imagePreviewPath), "%s", imgFileName);
        break;
      case DISP_ANIMATING:
        snprintf(displayModeText, sizeof(displayModeText), "Currently displaying this animation:");
        snprintf(imagePreviewPath, sizeof(imagePreviewPath), "/data/anim/%s/pvw.gif", animName);
        break;
      case DISP_CAMERA:
        snprintf(displayModeText, sizeof(displayModeText), "Currently displaying from camera: (not yet implemented)");
        snprintf(imagePreviewPath, sizeof(imagePreviewPath), "/web/res/displayOff.png");
        break;
    }
  }
  streamFileWithReplacements("/web/home.html", srcStrs, (const char**)repStrs, 9);
}

//FIXME: would be better to not repeat the code this much
void sendAnimPage() {
  char lineBuffer[MAX_HTML_LINE];
  int bytesRead;
  File dir = SD.open("/data/anim");
  File f;
  File htmlFile = SD.open("/web/select_animation.html", FILE_READ);
  if (!htmlFile) {
    WRITE_OUT("Failed to open /web/select_animation.html\n");
    server.send(404, "text/plain", "404: Not Found");
    return;
  }
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  while (htmlFile.available()) {
    bytesRead = htmlFile.readBytesUntil('\n', lineBuffer, MAX_HTML_LINE);
    if (!bytesRead) { continue; } //skip empty lines to avoid chunked content being terminated early
    lineBuffer[bytesRead] = '\n'; //add back in the newline
    lineBuffer[bytesRead+1] = '\0'; //add string terminator
    if (strcmp(lineBuffer, "ANIMATION_PREVIEW_CONTENTS\n") == 0) { //match
      while (f = dir.openNextFile()) {
        snprintf(lineBuffer, sizeof(lineBuffer), "<a href=\"/home?command=animate&animName=%s\"><img src=\"/data/anim/%s/pvw.gif\" style=\"width:100%%\" alt=\"Animation Preview\"></a>\n", f.name(), f.name());
        server.sendContent(lineBuffer);
      }
    }
    else {
      server.sendContent(lineBuffer);
    }
  }
  htmlFile.close();
  server.sendContent("");
  server.client().stop();
}
void sendImgPage() {
  char lineBuffer[MAX_HTML_LINE];
  int bytesRead;
  File dir = SD.open("/data/img");
  File f;
  File htmlFile = SD.open("/web/select_image.html", FILE_READ);
  if (!htmlFile) {
    WRITE_OUT("Failed to open /web/select_image.html\n");
    server.send(404, "text/plain", "404: Not Found");
    return;
  }
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  while (htmlFile.available()) {
    bytesRead = htmlFile.readBytesUntil('\n', lineBuffer, MAX_HTML_LINE);
    if (!bytesRead) { continue; } //skip empty lines to avoid chunked content being terminated early
    lineBuffer[bytesRead] = '\n'; //add back in the newline
    lineBuffer[bytesRead+1] = '\0'; //add string terminator
    if (strcmp(lineBuffer, "IMAGE_PREVIEW_CONTENTS\n") == 0) { //match
      while (f = dir.openNextFile()) {
        snprintf(lineBuffer, sizeof(lineBuffer), "<a href=\"/home?command=image&imgName=%s\"><img src=\"/data/img/%s\" style=\"width:100%%\" alt=\"Image Preview\"></a>\n", f.name(), f.name());
        server.sendContent(lineBuffer);
      }
    }
    else {
      server.sendContent(lineBuffer);
    }
  }
  htmlFile.close();
  server.sendContent("");
  server.client().stop();
}

void handleRoot() {
  //by default if not enough arguments are provided for a command, then no state change is made on the system.
  if ( server.hasArg("command") ) {
    if ( server.arg("command") == "off" ) {
      //no extra setup required
      displayMode = DISP_OFF;
    }
    else if ( server.arg("command") == "animate" ) {
      if ( server.hasArg("animName") ) {
        setupAnimationInterface(server.arg("animName").c_str());
      }
    }
    else if ( server.arg("command") == "image" ) {
      if ( server.hasArg("imgName") ) {
        if ( server.hasArg("scrollImg") && server.arg("scrollImg") == "true" ) {
          setupImageInterface(server.arg("imgName").c_str(), true);
        }
        else {
          setupImageInterface(server.arg("imgName").c_str(), false);
        }
      }
    }
    else if ( server.arg("command") == "text" ) {
      if ( server.hasArg("textInput") ) {
        if ( server.hasArg("scrollText") && server.arg("scrollText") == "true" ) {
          setupTextInterface(server.arg("textInput").c_str(), true);
        }
        else {
          setupTextInterface(server.arg("textInput").c_str(), false);
        }
      }
    }
  }
  sendHomePage();
}
void handleUpload() {
  HTTPUpload& upload = server.upload();
  char fname[MAX_FILE_NAME + 1];
  snprintf(fname, sizeof(fname), "/data/img/%s", upload.filename.c_str());
  if ( upload.status == UPLOAD_FILE_START ) {
    tmpUpFile = SD.open(fname, FILE_WRITE); // Open the file for writing in SD (create if it doesn't exist)
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if ( tmpUpFile ) {
      tmpUpFile.write(upload.buf, upload.currentSize); // write however many bytes we've currently received
    }
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (tmpUpFile) {
      tmpUpFile.close(); //close before changing image
      setupImageInterface(fname, false);
      server.sendHeader("Location", "/home"); //redirect client back home
      server.send(303);
    }
    else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
void handleRetrieve(String fileName) {
  char dataType[15];
  *dataType = '\0';
  if ( fileName.endsWith(".html") ) {
    strcpy(dataType, "text/html");
  }
  else if ( fileName.endsWith(".bmp") ) {
    strcpy(dataType, "image/bitmap");
  }
  else if ( fileName.endsWith(".gif") ) {
    strcpy(dataType, "image/gif");
  }
  else if ( fileName.endsWith(".jpg") ) {
    strcpy(dataType, "image/jpg");
  }
  else if ( fileName.endsWith(".png") ) {
    strcpy(dataType, "image/png");
  }
  if (dataType == "" || (!SD.exists(fileName))) { //only allow certain file types to be returned
    WRITE_OUT("retrieval of " + fileName + " was invalid!");
    server.send(404, "text/plain", "404: Not Found");
    return;
  }
  File file = SD.open(fileName.c_str(), FILE_READ);
  server.streamFile(file, dataType);
  file.close();
}
void setupWebInterface() {
  WRITE_OUT("Connecting to ");
  WRITE_OUT(ssid);
  WiFi.begin(ssid, password); //connects to the wifi (hopefully)
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    WRITE_OUT(".");
  }
  WRITE_OUT("");
  WRITE_OUT("WiFi connected\n");
  WRITE_OUT("IP address: ");
  WRITE_OUT(WiFi.localIP());
  WRITE_OUT("\n");

  if (MDNS.begin("led_server")) {
    WRITE_OUT("MDNS responder started for led_server\n");
  }

  server.on("/", handleRoot);
  server.on("/home", handleRoot);
  server.on("/select_image", sendImgPage);
  server.on("/select_animation", sendAnimPage);
  server.on("/upload_animation", handleRoot); //FIXME: currently not yet implemented an animation upload
  server.on("/upload_image", [](){ streamFile("/web/upload_image.html"); } );
  server.on("/text_display", [](){ streamFile("/web/text_display.html"); } );
  server.on("/upload", HTTP_GET, [](){ server.send(404, "text/plain", "404: Not Found"); });
  server.on("/upload", HTTP_POST, [](){ server.send(200); }, handleUpload);
  server.onNotFound([]() { handleRetrieve(server.uri()); });

  //start the actual web server
  server.begin();
  WRITE_OUT("HTTP Server Started");
}

void loopWebInterface() {
  server.handleClient();
  MDNS.update();
}
