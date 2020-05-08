
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h" //defines my network's SSID and PSK, sorry nefarious users but I can't make that public.

const char* ssid = STASSID;
const char* password = STAPSK;

File tmpUpFile; // a File object for reading in the file being transmitted
ESP8266WebServer server(80);

String fetchHomePage() {
  String retPage;
  File homePageFile = SD.open("/web/home.html", "r");
  if (!homePageFile) {
    //Serial.println("Failed to open the home page!");
    return "";
  }
  retPage = homePageFile.readString();
  homePageFile.close();
  switch(displayMode) {
    case DISP_OFF:
      retPage.replace("DISPLAY_MODE_TEXT", "Display is currently turned off! Select an option from the side pane to setup what you want to display.");
      retPage.replace("IMAGE_PREVIEW_PATH", "/web/res/displayOff.png");
      retPage.replace("SINGLE_IMAGE_DISPLAY", "default");
      retPage.replace("FIVE_IMAGE_DISPLAY", "none");
      break;
    case DISP_IMAGE:
      retPage.replace("DISPLAY_MODE_TEXT", "Currently displaying this image:");
      retPage.replace("IMAGE_PREVIEW_PATH", imgFileName);
      retPage.replace("SINGLE_IMAGE_DISPLAY", "default");
      retPage.replace("FIVE_IMAGE_DISPLAY", "none");
      break;
    case DISP_ANIMATING:
      retPage.replace("DISPLAY_MODE_TEXT", "Currently displaying this animation:");
      retPage.replace("IMAGE_PREVIEW_PATH", "/data/anim/" + animName + "/preview.gif");
      retPage.replace("SINGLE_IMAGE_DISPLAY", "default");
      retPage.replace("FIVE_IMAGE_DISPLAY", "none");
      //fixme, add the animation here
      break;
    case DISP_TEXT:
      retPage.replace("DISPLAY_MODE_TEXT", "Server is in text scroll mode, displaying the message \"" + textDisplay + "\"");
      retPage.replace("SINGLE_IMAGE_DISPLAY", "none");
      retPage.replace("FIVE_IMAGE_DISPLAY", "default");
      for (int i = 0; i < 5; i++) {
        retPage.replace("IMAGE_" + String(i), "/data/font/" + textDisplay.charAt(i) + ".bmp");
      }
      break;
    case DISP_CAMERA:
      retPage.replace("DISPLAY_MODE_TEXT","Currently displaying from camera: (not yet implemented)");
      //fixme, add the camera frame preview here?
      retPage.replace("SINGLE_IMAGE_DISPLAY", "none");
      retPage.replace("FIVE_IMAGE_DISPLAY", "none");
      break;
  }
  return retPage;
}
String fetchAnimPage() {
  String retPage;
  File animPageFile = SD.open("/web/select_animation.html", "r");
  if (!animPageFile) {
    //Serial.println("Failed to open the home page!");
    return "";
  }
  retPage = animPageFile.readString();
  animPageFile.close();
  String tmpHTMLtoAdd = "";
  Dir animDir = SD.openDir("/data/anim");
  String animName;
  while (animDir.next()) {
    animName = animDir.fileName();
    tmpHTMLtoAdd += "<a href=\"/home?command=animate&animName=" + animName + "\">";
    tmpHTMLtoAdd += "<img src=\"/data/anim/" + animName + "/preview.gif\" style=\"width:100%\" alt=\"Animation Preview\">";
    tmpHTMLtoAdd += "</a>\n";
  }
  retPage.replace("ANIMATION_PREVIEW_CONTENTS", tmpHTMLtoAdd);
  return retPage;
}
String fetchImgPage() {
  String retPage;
  File imgPageFile = SD.open("/web/select_image.html", "r");
  if (!imgPageFile) {
    //Serial.println("Failed to open the home page!");
    return "";
  }
  retPage = imgPageFile.readString();
  imgPageFile.close();
  String tmpHTMLtoAdd = "";
  Dir imgDir = SD.openDir("/data/img");
  String imgName;
  while (imgDir.next()) {
    imgName = imgDir.fileName();
    tmpHTMLtoAdd += "<a href=\"/home?command=image&imgName=" + imgName + "\">";
    tmpHTMLtoAdd += "<img src=\"/data/img/" + imgName + "\" style=\"width:100%\" alt=\"Image Preview\">";
    tmpHTMLtoAdd += "</a>\n";
  }
  retPage.replace("IMAGE_PREVIEW_CONTENTS", tmpHTMLtoAdd);
  return retPage;
}

void handleRoot() {
  //by default if not enough arguments are provided for a command, then no state change is made on the system.
  if ( server.hasArg("command") ) {
    if ( server.arg("command") == "off" ) {
      //no extra setup required
      displayMode = DISP_OFF;
    }
    else if ( server.arg("command") == "animate" ) {
      if ( server.hasArg("animName") {
        setupAnimationInterface(server.arg("animName"));
      }
    }
    else if ( server.arg("command") == "image" ) {
      // if ( server.hasArg("scroll") && server.hasArg("imgName") ) {
      if ( server.hasArg("imgName") ) { //FIXME scrolling not yet enabled
        setupImageInterface(server.arg("imgName"), false);
      }
    }
    else if ( server.arg("command") == "text" ) {
      if ( server.hasArg("scrollText") && server.hasArg("textInput") ) {
        setupTextInterface(server.arg("textInput"), server.arg("scroll") == "true");
      }
    }
  }
  server.send(200, "text/html", fetchHomePage());
}
void handleUpload() {
  HTTPUpload& upload = server.upload();
  String filename = upload.filename;
  if (filename.startsWith("/")) filename = filename.substring(1,-1);
  filename = "/data/img/" + filename;
  if ( upload.status == UPLOAD_FILE_START ) {
    tmpUpFile = SD.open(filename, "w"); // Open the file for writing in SD (create if it doesn't exist)
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if ( tmpUpFile ) {
      tmpUpFile.write(upload.buf, upload.currentSize); // write however many bytes we've currently received
    }
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (tmpUpFile) {
      tmpUpFile.close(); //close before changing image
      setupImageInterface(filename, false);
      server.sendHeader("Location", "/home"); //redirect client back home
      server.send(303);
    }
    else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
void handleRetrieve(String fileName) {
  //Serial.println("handleImgRetrieve: " + fileName);
  String dataType = "";
  if ( fileName.endsWith(".html") ) {
    dataType = "text/html";
  }
  else if ( fileName.endsWith(".bmp") ) {
    dataType = "image/bitmap";
  }
  else if ( fileName.endsWith(".gif") ) {
    dataType = "image/gif";
  }
  else if ( fileName.endsWith(".jpg") ) {
    dataType = "image/jpg";
  }
  else if ( fileName.endsWith(".png") ) {
    dataType = "image/png";
  }
  if (dataType == "" || (!SD.exists(fileName))) { //only allow certain file types to be returned
    server.send(404, "text/plain", "404: Not Found");
    return;
  }
  File file = SD.open(fileName, "r");
  server.streamFile(file, dataType);
  file.close();
  //Serial.println(String("\tSent file: ") + fileName);
}
void setupWebInterface() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password); //connects to the wifi (hopefully)
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/home", handleRoot);
  server.on("/text_display", [](){ File f = SD.open("/web/text_display.html"); server.streamFile(f, "text/html"); f.close(); });
  server.on("/upload", HTTP_GET, [](){ server.send(404, "text/plain", "404: Not Found"); });
  server.on("/upload", HTTP_POST, [](){ server.send(200); }, handleUpload);
  server.onNotFound([]() { handleRetrieve(server.uri()); });

  //start the actual web server
  server.begin();
  Serial.println("HTTP Server Started");
}

void loopWebInterface() {
  server.handleClient();
  MDNS.update();
}
