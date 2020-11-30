#ifndef __SNOWFOX_H__
#define __SNOWFOX_H__

#include "quantum.h"
#include "ch.h"
#include "sled17341.h"

extern THD_WORKING_AREA(waLEDThread, 128);
THD_FUNCTION(LEDThread, arg);

#endif
