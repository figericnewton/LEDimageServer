
#ifndef DEBUGGING_H
#define DEBUGGING_H

//uncomment as desired when debugging
#define DEBUG
//#define TIMER_ENABLED

#ifdef DEBUG
  #define WRITE_OUT(x) Serial.print(x)
  void echoFreeMemory();
#else
  #define WRITE_OUT(x) 
  #define echoFreeMemory(x)   
#endif

#ifdef TIMER_ENABLED
  void startTimer();
  void stopTimer();
#else
  #define startTimer(x)  
  #define stopTimer(x)  
#endif

#endif
