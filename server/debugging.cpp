
#ifndef DEBUGGING_CPP
#define DEBUGGING_CPP

#include "globals.h"

#ifdef TIMER_ENABLED
long lastTime = 0;
void startTimer() {
  lastTime = millis();
}
void stopTimer() {
  long curTime = millis();
  WRITE_OUT("timer: ");
  WRITE_OUT(curTime - lastTime);
  WRITE_OUT("millis\n");
  lastTime = curTime;
}
#endif

#endif
