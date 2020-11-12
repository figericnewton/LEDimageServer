
#include "globals.h"
#include "website.h"

#define CS_PIN D8

const RgbColor Black(0);
const RgbColor Red(120, 0, 0);
const RgbColor Green(0, 120, 0);
const RgbColor Blue(0, 0, 120);

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> topPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO3
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> midPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO2
#ifndef DEBUG
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod> botPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO1 (which conflicts with serial monitoring)
#endif

NeoTopology<RowMajorAlternatingLayout> topo(PANEL_WIDTH, PANEL_HEIGHT/3);
uint16_t MyLayoutMap(int16_t x, int16_t y) {
  return topo.MapProbe(x, y);
}

uint8_t currentFrameBuffer[BYTES_PER_PIX * PANEL_WIDTH * PANEL_HEIGHT];
NeoBuffer<NeoBufferProgmemMethod<NeoGrbFeature>> neoPixFrameBuffer(PANEL_WIDTH, PANEL_HEIGHT, currentFrameBuffer);

void setup() {
#ifdef DEBUG
  Serial.begin(74880);
  delay(10);
#endif

#ifndef DEBUG
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
  setupAllOperatingModes(server);
  startTimer();
}
void loop() {
  loopWebInterface();
  CurrentOperatingMode->updateFrame(currentFrameBuffer, &neoPixFrameBuffer);
  neoPixFrameBuffer.Blt(topPixels, 0, 0, 0,                0, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
  neoPixFrameBuffer.Blt(midPixels, 0, 0, 0,   PANEL_HEIGHT/3, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
#ifndef DEBUG
  neoPixFrameBuffer.Blt(botPixels, 0, 0, 0, 2*PANEL_HEIGHT/3, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
#endif

  //FIXME: include logic to set an explicit refresh rate
  topPixels.Show();
  midPixels.Show();
  //note Uart0800 (botPixels) is the same as Serial debug port, can't use both at the same time
#ifndef DEBUG
  botPixels.Show();
#endif
  stopTimer();
}
