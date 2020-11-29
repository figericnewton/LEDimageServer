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
    WRITE_OUT("ws[%s][%u] connect\n", server->url(), client->id());
  } else if(type == WS_EVT_DISCONNECT) {
    WRITE_OUT("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if (info->len == frameBufferCTX.SizePixels) { //client is sending us the video data
      if ( CurrentOperatingMode != &StreamOperatingMode ) {
        CurrentOperatingMode = &StreamOperatingMode;
      }
      WRITE_OUT("RECEIVED (%u/%u)\n", (info->index + len), info->len);
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
    WRITE_OUT("ws[%s][%u] unhandled event type (ignored)\n", server->url(), client->id());
  }
}
#endif
