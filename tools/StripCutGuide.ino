
//Used to test out the strips and mark where to cut them in half.

#include <NeoPixelBus.h>

const uint16_t PixelCount = 150;
const uint8_t PixelPin = 2;  // this is ignored by NeoPixelBus library for Esp8266

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(128, 0, 0);
RgbColor green(0, 128, 0);
RgbColor blue(0, 0, 128);
RgbColor white(128);
RgbColor black(0);

void setup() {
  Serial.begin(74880);
  delay(10);
  strip.Begin(); //initialize the LED strip
  strip.Show(); //clear any currently lit up LEDs

  strip.SetPixelColor(0, red);
  strip.SetPixelColor(74,green);
  strip.SetPixelColor(75,blue);
  strip.SetPixelColor(149,white);
  strip.Show();
}

void loop() {
  strip.SetPixelColor(0, red);
  strip.SetPixelColor(74,green);
  strip.SetPixelColor(75,blue);
  strip.SetPixelColor(149,white);
  strip.Show();
  delay(1000);
}
