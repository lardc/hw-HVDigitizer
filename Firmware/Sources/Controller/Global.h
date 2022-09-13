// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
//
#include "stdinc.h"

// Definitions
//
#define OE_MASK_R1		BIT1	// External Relay1 (connect DC)
#define OE_MASK_R2		BIT2	// External Relay2 (shunt diode for AC)
#define OE_MASK_IDC		BIT3	// Internal relay for DC sensing
#define OE_MASK_IDC_H	BIT4	// DC sensing high range
#define OE_MASK_RDC		BIT5	// Relay for RD-board
//
#define ADC_OFFSET_IQ	_IQ(0)

#endif // __GLOBAL_H
