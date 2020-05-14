//the intent of this program is to provide a web interface for updating an LED display

#include <NeoPixelBus.h>
#include <SPI.h>
#include <SD.h>

//uncomment when debugging
#define DEBUG

#ifdef DEBUG
  #define WRITE_OUT(x) Serial.print(x)
#else
  #define WRITE_OUT(x)
#endif

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

//SPIFFS and LittleFS limit to 31 characters in a file name (32 including \0)
#define MAX_FILE_NAME 31

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
#ifdef DEBUG
  Serial.begin(74880);
  delay(10);
#endif

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
  //note Uart0800 (botPixels) is the same as Serial debug port, can't use both at the same time
#ifndef DEBUG
  botPixels.Show();
#endif
}
