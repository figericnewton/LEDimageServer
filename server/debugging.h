
#ifndef DEBUGGING_H
#define DEBUGGING_H

//uncomment as desired when debugging
#define DEBUG
//#define DEBUG_TIMER_ENABLED
//#define DEBUG_MEMORY_ENABLED

#ifdef DEBUG
  #define WRITE_OUT(...) Serial.printf(__VA_ARGS__)
#else
  #define WRITE_OUT(...)  
#endif

#ifdef DEBUG_TIMER_ENABLED
  void startTimer();
  void stopTimer();
#else
  #define startTimer(x)  
  #define stopTimer(x)  
#endif

#ifdef DEBUG_MEMORY_ENABLED
  void echoFreeMemory();
#else
  #define echoFreeMemory(x)   
#endif

#endif
