// -----------------------------------------
// Auxiliary sampling routines
// ----------------------------------------

#ifndef __SAMPLING_H
#define __SAMPLING_H

// Include
#include "stdinc.h"
#include "IQmathLib.h"

// Functions
//
// Set ADC channels
void SAMPLING_SetChannels(Int16U CurrentIn, Int16U VoltageIn);
// Reset ADC launches counter
void SAMPLING_ResetCounter();
// Collect ADC data
Boolean SAMPLING_LoadData(Int16U * const restrict aSampleVector);
// Output collected data
void SAMPLING_DataOutput(_iq *Voltage, _iq *Current);

#endif // __SAMPLING_H
