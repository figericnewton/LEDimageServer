//the intent of this program is to provide a web interface for updating an LED display

#include <NeoPixelBus.h>
#include <SPI.h>
#include <SD.h>

#define PANEL_WIDTH 75
#define PANEL_HEIGHT 36
#define BYTES_PER_PIX 3
#define BYTES_PER_PIX 3
#define CS_PIN D8

#define DISP_OFF 0
#define DISP_IMAGE 1
#define DISP_ANIMATING 2
#define DISP_TEXT 3
#define DISP_CAMERA 4
uint8_t displayMode = DISP_OFF;

const RgbColor Black(0);
const RgbColor Red(120, 0, 0);
const RgbColor Green(0, 120, 0);
const RgbColor Blue(0, 0, 120);

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> topPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO3
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> midPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO2
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod> botPixels(PANEL_WIDTH * PANEL_HEIGHT/3); //uses GPIO1
      
NeoTopology<RowMajorAlternatingLayout> topo(PANEL_WIDTH, PANEL_HEIGHT);
uint16_t MyLayoutMap(int16_t x, int16_t y) {
  return topo.MapProbe(x, y);
}

void setup() {
  //note that serial debigging can't be used when displaying to both GPIO1 and GPIO2
  //Serial.begin(115200);
  //delay(10);

  botPixels.Begin();
  delay(10);
  topPixels.Begin();
  delay(10);
  midPixels.Begin();
  delay(10);

  SD.begin(CS_PIN); //start the file system

  setupWebInterface();
}

void loop() {
  loopWebInterface();
  //imageScroll();
  switch(displayMode) {
    case DISP_OFF:
      topPixels.ClearTo(Black);
      midPixels.ClearTo(Black);
      botPixels.ClearTo(Black);
      break;
    case DISP_IMAGE:
      loopImageInterface();
      break;
    case DISP_ANIMATING:
      loopAnimationInterface();
      break;
    case DISP_TEXT:
      loopTextInterface();
      break;
    case DISP_CAMERA:
      //FIXME: not yet implemented
      break;
  }
  topPixels.Show();
  midPixels.Show();
  botPixels.Show();
  //stopMillis = millis();
  //Serial.println("Computational time to make show calls: " + String(stopMillis-startMillis));
  //startMillis = millis();
}
