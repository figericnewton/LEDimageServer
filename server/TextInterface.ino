
String textDisplay;
uint8_t scrollText = false;

void setupTextInterface(String text, uint8_t scroll) {
  textDisplay = text;
  scrollText = scroll;
  displayMode = DISP_TEXT;
}
void loopTextInterface() {
  //FIXME
}
