
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
  WRITE_OUT("timer: ");
  WRITE_OUT(curTime - lastTime);
  WRITE_OUT("millis\n");
  lastTime = curTime;
}
#endif

#ifdef DEBUG_MEMORY_ENABLED
uint8_t * heapptr, * stackptr;
void check_mem() {
  int SP;
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the memory again (sets stackptr to 0)
  stackptr =  (uint8_t *)(&SP);           // save value of stack pointer
}
void echoFreeMemory() {
  check_mem();
  WRITE_OUT("memory left: ");
  WRITE_OUT(stackptr - heapptr);
  WRITE_OUT("\n");
}
#endif
#endif
