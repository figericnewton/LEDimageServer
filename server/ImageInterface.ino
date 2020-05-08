
String imgFileName;
uint8_t scrollImg = false;
uint16_t imageScrollOffset = 0;
NeoBitmapFile<NeoGrbFeature, File> imageBMP;

// define a custom shader object that does the gamma correction
NeoGamma<NeoGammaTableMethod> colorGamma;
class GammaCorrectionShader : public NeoShaderBase {
  public:
    RgbColor Apply(uint16_t index, RgbColor original) {
      return colorGamma.Correct(original);
    }
};
GammaCorrectionShader gammaCorrectionShader;
void setupImageInterface(String fileName, uint8_t scroll) {
  File f = SD.open(fileName);
  if (!f) {
    //Serial.println("Failed to open: " + imgFileName);
    return;
  }
  if (!imageBMP.Begin(f)) {
    //Serial.println("File format error trying to use " + imgFileName);
    return;
  }
  //only update state information if file read succeeded
  imgFileName = fileName;
  scrollImg = scroll;
  imageScrollOffset = 0; //always start this at 0
  //FIXME, could have speed improvement by only doing gamma correction once instead of all the time
  imageBMP.Render<GammaCorrectionShader>(topPixels, colorGamma, 0, 0, 0,                    0, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(midPixels, colorGamma, 0, 0, 0,   PANEL_HEIGHT/3 - 1, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(botPixels, colorGamma, 0, 0, 0, 2*PANEL_HEIGHT/3 - 1, PANEL_WIDTH, PANEL_HEIGHT/3, MyLayoutMap);
  displayMode = DISP_IMAGE;
}
void loopImageInterface() {
  if ( !scrollImg ) { //if we're not scrolling the image then there's nothing to do
    return;
  }
  imageScrollOffset++;
  if (imageScrollOffset == image.Width()) {
    imageScrollOffset = 0;
  }
  imageBMP.Render<GammaCorrectionShader>(topPixels, colorGamma, imageScrollOffset, 0,                               0,                    0, PANEL_WIDTH - imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(topPixels, colorGamma,                 0, 0, PANEL_WIDTH - imageScrollOffset,                    0,               imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(midPixels, colorGamma, imageScrollOffset, 0,                               0,   PANEL_HEIGHT/3 - 1, PANEL_WIDTH - imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(midPixels, colorGamma,                 0, 0, PANEL_WIDTH - imageScrollOffset,   PANEL_HEIGHT/3 - 1,               imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(botPixels, colorGamma, imageScrollOffset, 0,                               0, 2*PANEL_HEIGHT/3 - 1, PANEL_WIDTH - imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
  imageBMP.Render<GammaCorrectionShader>(botPixels, colorGamma,                 0, 0, PANEL_WIDTH - imageScrollOffset, 2*PANEL_HEIGHT/3 - 1,               imageScrollOffset, PANEL_HEIGHT/3, MyLayoutMap);
}
