
#ifndef PONG_CPP
#define PONG_CPP

#include "globals.h"

void video__setup(AsyncWebServer* server);
void video__processRequest(AsyncWebServerRequest* request);
void video__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void video__updateFrame();

OperatingMode VideoOperatingMode = {
  .setup = video__setup,
  { .prevPath = "/web/res/displayOff.png" }, //FIXME: need to generate a preview image?
  .updateFrame = video__updateFrame,
};

AsyncWebSocket video__ws("/video");

void video__updateFrame() {
  video__ws.cleanupClients(1); //ONLY ONE CLIENT AT A TIME CAN STREAM!!!
}
void video__setup(AsyncWebServer* server) {
  video__ws.onEvent(video__wsEvent);
  server->addHandler(&video__ws);
  server->serveStatic("/video_stream.html", SDFS, "/web/video_stream.html");
}
void video__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if (type == WS_EVT_CONNECT) {
    WRITE_OUT("ws[");
    WRITE_OUT(server->url());
    WRITE_OUT("][");
    WRITE_OUT(client->id());
    WRITE_OUT("] connect\n");
  } else if(type == WS_EVT_DISCONNECT) {
    WRITE_OUT("ws[");
    WRITE_OUT(server->url());
    WRITE_OUT("][");
    WRITE_OUT(client->id());
    WRITE_OUT("] disconnect\n");
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if (info->len == frameBufferCTX.SizePixels) { //client is sending us the video data
      if ( CurrentOperatingMode != &VideoOperatingMode ) {
        CurrentOperatingMode = &VideoOperatingMode;
      }
      WRITE_OUT("RECEIVED (");
      WRITE_OUT((const int)(info->index + len));
      WRITE_OUT("/");
      WRITE_OUT((const int)info->len);
      WRITE_OUT(")\n");
      memcpy(&frameBufferCTX.Pixels[info->index], data, len);
      if (info->index + len == info->len) {
        WRITE_OUT("requesting more data\n");
        client->binary("more plzzzz"); //request the next frame!
      }
    } else { //we're expected to send the client data
      WRITE_OUT("SENDING TO CLIENT\n");
      client->binary(frameBufferCTX.Pixels, frameBufferCTX.SizePixels);
    }
  } else {
    WRITE_OUT("ws[");
    WRITE_OUT(server->url());
    WRITE_OUT("][");
    WRITE_OUT(client->id());
    WRITE_OUT("] unhandled event type (ignored)\n");
  }
}
#endif
