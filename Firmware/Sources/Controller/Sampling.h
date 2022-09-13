// ----------------------------------------
// Auxiliary sampling routines
// ----------------------------------------

#ifndef __SAMPLING_H
#define __SAMPLING_H

// Include
#include "stdinc.h"

// Functions
//
// Set ADC channels
void SAMPLING_SetChannels(Int16U CurrentIn, Int16U VoltageIn);
// Collect ADC data
void SAMPLING_LoadData(Int16U * const restrict aSampleVector, pInt32U Voltage, pInt32U Current);

#endif // __SAMPLING_H
