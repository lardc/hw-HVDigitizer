// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "IQmathLib.h"

// Types
//
typedef void (*CONTROL_FUNC_RealTimeRoutine)();
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern volatile Boolean CONTROL_MeasurementActive;
extern _iq SampledCurrent, SampledVoltage;

// Functions
//
// Initialize controller
void CONTROL_Init();
// Do background idle operation
void CONTROL_Idle();
// Delegate low priority task
void CONTROL_RequestDPC(FUNC_AsyncDelegate Action);
// Handle master packet
void CONTROL_HandleTransmission();
// Update high-priority states
void CONTROL_UpdateHigh();
// Re-init RX SPI channels
void CONTROL_ReInitSPI_Rx();
// Subscribe to real-time routine
void CONTROL_SubcribeToCycle(CONTROL_FUNC_RealTimeRoutine Routine);

#endif // __CONTROLLER_H
