
#ifndef WEBSITE_CPP
#define WEBSITE_CPP

#include "website.h"
#include "credentials.h" //defines my network's SSID and PSK, sorry nefarious users but I can't make that public.

const char* ssid = STASSID;
const char* password = STAPSK;

AsyncWebServer server(80);
/*
AsyncWebSocket ws("/ws");
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}
*/
String homeTemplateProcessor(const String& var) {
  WRITE_OUT("template processor called to replace: ");
  WRITE_OUT(var);
  WRITE_OUT("\n");
  if (var == F("DISPLAY_MODE_TEXT")) {
    //FIXME: consider creating text for each different display mode instead of this generic text
    return F("Preview of the LED display:");
  } else if ( var == "IMAGE_PREVIEW_PATH" ) {
    return String(CurrentOperatingMode->prevPath);
  }
  return String();
}
AsyncWebServer* setupWebInterface() {
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

  //ws.onEvent(onWsEvent);
  //server.addHandler(&ws);
  
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
