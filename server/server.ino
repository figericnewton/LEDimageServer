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

#include "globals.h"
#include "website.h"

#define CS_PIN D8

const RgbColor Black(0);
const RgbColor Red(120, 0, 0);
const RgbColor Green(0, 120, 0);
const RgbColor Blue(0, 0, 120);

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> topPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO3
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> midPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO2
#if !DEBUG
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod> botPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO1 (which conflicts with serial monitoring)
#endif

NeoTopology<RowMajorAlternatingLayout> topo(PANEL_WIDTH, PANEL_HEIGHT/3);
uint16_t MyLayoutMap(int16_t x, int16_t y) {
  return topo.MapProbe(x, y);
}

NeoBuffer<NeoBufferMethod<NeoGrbFeature>> neoPixFrameBuffer(PANEL_WIDTH, PANEL_HEIGHT, NULL);
NeoBufferContext<NeoGrbFeature> frameBufferCTX = neoPixFrameBuffer;

void setup() {
#if DEBUG
  Serial.begin(74880);
  delay(10);
#else
  botPixels.Begin();
  delay(10);
#endif

  topPixels.Begin();
  delay(10);
  midPixels.Begin();
  delay(10);

  SDFS.setConfig(SDFSConfig(CS_PIN, SD_SCK_MHZ(50)));
  SDFS.begin(); //start the file system

  AsyncWebServer* server = setupWebInterface();
  startTimer();
}
void loop() {
  CurrentOperatingMode->updateFrame();
  neoPixFrameBuffer.Blt(topPixels, 0, 0, 0,                0, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
  neoPixFrameBuffer.Blt(midPixels, 0, 0, 0,   PANEL_HEIGHT/3, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
#if !DEBUG
  neoPixFrameBuffer.Blt(botPixels, 0, 0, 0, 2*PANEL_HEIGHT/3, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
#endif

  //FIXME: include logic to set an explicit refresh rate
  topPixels.Show();
  midPixels.Show();
  //note Uart0800 (botPixels) is the same as Serial debug port, can't use both at the same time
#if !DEBUG
  botPixels.Show();
#endif
  stopTimer();
  echoFreeMemory();
}
