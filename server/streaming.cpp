
#ifndef STREAMING_CPP
#define STREAMING_CPP

#include "globals.h"

void stream__setup(AsyncWebServer* server);
void stream__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void stream__updateFrame();

OperatingMode StreamOperatingMode = {
  .setup = stream__setup,
  { .prevPath = "/web/res/displayOff.png" }, //FIXME: need to generate a preview image?
  .updateFrame = stream__updateFrame,
};

AsyncWebSocket stream__ws("/stream");

void stream__updateFrame() {
  stream__ws.cleanupClients(1); //ONLY ONE CLIENT AT A TIME CAN STREAM!!!
}
void stream__setup(AsyncWebServer* server) {
  stream__ws.onEvent(stream__wsEvent);
  server->addHandler(&stream__ws);
  server->serveStatic("/video_stream.html", SDFS, "/web/video_stream.html");
  server->serveStatic("/audio_visualizer.html", SDFS, "/web/audio_visualizer.html");
  server->serveStatic("/text_display.html", SDFS, "/web/text_display.html");
  server->serveStatic("/drawing.html", SDFS, "/web/drawing.html");
}
void stream__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
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
      if ( CurrentOperatingMode != &StreamOperatingMode ) {
        CurrentOperatingMode = &StreamOperatingMode;
      }
      WRITE_OUT("RECEIVED (");
      WRITE_OUT((const int)(info->index + len));
      WRITE_OUT("/");
      WRITE_OUT((const int)info->len);
      WRITE_OUT(")\n");
      memcpy(&frameBufferCTX.Pixels[info->index], data, len);
      if (info->index + len == info->len) {
        WRITE_OUT("requesting more data\n");
        client->binary("OK"); //request the next frame!
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
