
#define NUM_CHARACTERS_IN_FONT 38
#define MAX_TEXT_LENGTH 100
char textDisplay[MAX_TEXT_LENGTH + 1];
uint16_t textLength;
uint16_t textScrollOffset;
uint8_t scrollText = false;

const char characters[] = "_.ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
File fontFiles[NUM_CHARACTERS_IN_FONT];
NeoBitmapFile<NeoGrbFeature, File> fontImg[NUM_CHARACTERS_IN_FONT];
NeoBitmapFile<NeoGrbFeature, File> *textImg[MAX_TEXT_LENGTH];

uint8_t fontLoaded = false;
void loadFont() {
  char fname[MAX_FILE_NAME + 1];
  if (fontLoaded) { return; }
  for (uint8_t i = 0; i < NUM_CHARACTERS_IN_FONT; i++) {
    snprintf(fname, sizeof(fname), "/data/font/%c.bmp", characters[i]);
    fontFiles[i] = SD.open(fname, FILE_READ);
    if (!fontFiles[i]) {
      WRITE_OUT(fname);
      WRITE_OUT("could not be opened!\n");
    }
    if (!fontImg[i].Begin(fontFiles[i])) {
      WRITE_OUT("File format error trying to use");
      WRITE_OUT(fname);
      WRITE_OUT("\n");
    }
  }
  fontLoaded = true;
  WRITE_OUT("Font loaded!\n");
}
void setupTextInterface(const char* text, uint8_t scroll) {
  const char *chPtr;
  loadFont();
  snprintf(textDisplay, sizeof(textDisplay), text);
  while ( (textLength = strlen(textDisplay)) < 5 ) { strcat(textDisplay, "_"); }
  scrollText = scroll;
  textScrollOffset = -4; //start first character on right side of the screen
  for (uint8_t i = 0; i < textLength; i++) {
    if (!(chPtr = strchr(characters, textDisplay[i]))) { chPtr = characters; } //find char in array, default to "_" if not found
    textImg[i] = &fontImg[(chPtr - characters)];
  }
  displayMode = DISP_TEXT;
}
void loopTextInterface() {
  if (!scrollText) { return; } //if we're not scrolling the text then there's nothing to do
  textScrollOffset++;
  if (textScrollOffset == textLength) { textScrollOffset = 0; }
  for (uint8_t i = 0; i < 5; i++) {
    uint8_t txtPos = textScrollOffset + i;
    if (txtPos < 0 || txtPos >= textLength) { //render blank at the beginning and end
      fontImg[0].Blt(topPixels, i*PANEL_WIDTH/5, 0, 0,                0, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
      fontImg[0].Blt(midPixels, i*PANEL_WIDTH/5, 0, 0,   PANEL_HEIGHT/3, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
      fontImg[0].Blt(botPixels, i*PANEL_WIDTH/5, 0, 0, 2*PANEL_HEIGHT/3, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
    }
    else {
      textImg[txtPos]->Blt(topPixels, i*PANEL_WIDTH/5, 0, 0,                0, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
      textImg[txtPos]->Blt(midPixels, i*PANEL_WIDTH/5, 0, 0,   PANEL_HEIGHT/3, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
      textImg[txtPos]->Blt(botPixels, i*PANEL_WIDTH/5, 0, 0, 2*PANEL_HEIGHT/3, PANEL_WIDTH/5, PANEL_HEIGHT/3, MyLayoutMap);
    }
  }
}
