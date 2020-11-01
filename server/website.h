
#ifndef WEBSITE_H
#define WEBSITE_H

#include "globals.h"

//call once to do the initial website setup... starts server and such
AsyncWebServer* setupWebInterface();

//call periodically throughout execution
void loopWebInterface();

#endif
