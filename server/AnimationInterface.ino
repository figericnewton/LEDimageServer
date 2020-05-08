

uint8_t curSpriteFrame[BYTES_PER_PIX * PANEL_WIDTH * PANEL_HEIGHT];
NeoVerticalSpriteSheet<NeoBufferProgmemMethod<NeoGrbFeature>> curSprites(
      PANEL_WIDTH,    // image width and sprite width since its vertical sprite sheet
      PANEL_HEIGHT,   // image height
      PANEL_HEIGHT/3, // divide each frame into thirds to display using separate pins
      curSpriteFrame);
      
uint16_t numFrames;
uint16_t frame = 1;
String animName;
void setupAnimationInterface(String aName) {
  animName = aName;
  frame = 1;
  numFrames = 160; //FIXME this should be pulled from a metafile of some sort
  
  displayMode = DISP_ANIMATING;
}
void loopAnimationInterface() {
  String fname;
  fname = "/data/anim/" + animName + "/" + String(frame) + ".raw_grb";
  File imgFile = SD.open(fname, "r"); //open the image frame
  if (!imgFile) {
    //Serial.println("file open failed");
    return;
  }
  imgFile.readBytes((char *)curSpriteFrame, BYTES_PER_PIX * PANEL_WIDTH * PANEL_HEIGHT); //copy GRB data to sprite sheet data buffer
  imgFile.close();

  //transfer from sprite sheet to the three buffers for our NeoPixelBus's using the layout mapping
  curSprites.Blt(topPixels, 0, 0, 0, MyLayoutMap);
  curSprites.Blt(midPixels, 0, 0, 1, MyLayoutMap);
  curSprites.Blt(botPixels, 0, 0, 2, MyLayoutMap);

  frame++;
  if (frame > numFrames) {
    frame = 1;
  }
}
