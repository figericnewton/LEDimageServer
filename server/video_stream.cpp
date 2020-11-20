
#ifndef PONG_CPP
#define PONG_CPP

#include "globals.h"

void video__setup(AsyncWebServer* server);
void video__processRequest(AsyncWebServerRequest* request);
void video__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void video__updateFrame(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> *neoPixFrameBuffer);

OperatingMode VideoOperatingMode = {
  .setup = video__setup,
  { .prevPath = "/web/res/displayOff.png" }, //FIXME: need to generate a preview image?
  .updateFrame = video__updateFrame,
};

AsyncWebSocket video__ws("/video");

void video__updateFrame(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> *neoPixFrameBuffer) {
  //WRITE_OUT("video__updateFrame\n");
  //FIXME
}
int skipCount = 0;
AsyncWebSocketClient *activeClient = NULL;
void video__streamToClients(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> *neoPixFrameBuffer) {
  video__ws.cleanupClients();
  if (skipCount++ < 5 || !activeClient) { return; } //try slowing down the send rate a lot
  skipCount = 0;
  WRITE_OUT("Sending pixel data?\n");
  NeoBufferContext<NeoGrbFeature> ctx = *neoPixFrameBuffer;
  activeClient->binary(ctx.Pixels, ctx.SizePixels);
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
    activeClient = client;
  } else if(type == WS_EVT_DISCONNECT) {
    activeClient = NULL;
    WRITE_OUT("ws[");
    WRITE_OUT(server->url());
    WRITE_OUT("][");
    WRITE_OUT(client->id());
    WRITE_OUT("] disconnect\n");
  } else if (type == WS_EVT_DATA) {
    // AwsFrameInfo * info = (AwsFrameInfo*)arg;
    // StaticJsonDocument<256> msgJSON;
    // if (info->final && info->index == 0 && info->len == len) {
      // //the whole message is in a single frame and we got all of its data
      // WRITE_OUT("ws[");
      // WRITE_OUT(server->url());
      // WRITE_OUT("][");
      // WRITE_OUT(client->id());
      // WRITE_OUT("] received:\n");
      // for(size_t i=0; i < info->len; i++) {
        // WRITE_OUT((char) data[i]);
      // }
      // WRITE_OUT("\n");
      // if (deserializeJson(msgJSON, data)) {
        // WRITE_OUT("Failed to deserialize client message!\n");
        // return;
      // }
      // gameStateInfo.demand[msgJSON["player"].as<int>()] = msgJSON["demand"].as<int>();
    // }
  } else {
    WRITE_OUT("ws[");
    WRITE_OUT(server->url());
    WRITE_OUT("][");
    WRITE_OUT(client->id());
    WRITE_OUT("] unhandled event type (ignored)\n");
  }
}
#endif
