// ----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
#include "ZwDSP.h"

// Variables
//
extern volatile Int64U CONTROL_TimeCounter;

// Functions
//
// Initialize controller
void CONTROL_Init();
// Do background idle operation
void CONTROL_Idle();
// Handle master packet
void CONTROL_HandleTransmission();
// Re-init RX SPI channels
void CONTROL_ReInitSPI_Rx();

#endif // __CONTROLLER_H
