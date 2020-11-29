
#ifndef DEBUGGING_CPP
#define DEBUGGING_CPP

#include "globals.h"

#ifdef DEBUG_TIMER_ENABLED
long lastTime = 0;
void startTimer() {
  lastTime = millis();
}
void stopTimer() {
  long curTime = millis();
  WRITE_OUT("timer: %lu millis\n", curTime - lastTime);
  lastTime = curTime;
}
#endif

#ifdef DEBUG_MEMORY_ENABLED
void echoFreeMemory() {
  void *stackptr;
  void *heapptr = (uint8_t *)malloc(4);
  WRITE_OUT("memory left: %lu\n", &stackptr - heapptr);
  free(stackptr);
}
#endif
#endif
