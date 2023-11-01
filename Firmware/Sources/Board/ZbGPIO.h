// -----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

#ifndef __ZBGPIO_H
#define __ZBGPIO_H

// Include
#include "stdinc.h"
#include "ZwDSP.h"

// Functions
//
void ZbGPIO_Init();
void ZbGPIO_OECompose(Int16U Mask, Boolean Enable);
void ZbGPIO_OEReset();
void ZbGPIO_OEFlush(Boolean Force);

void inline ZbGPIO_SwitchLED1(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED_1, Set);
}
// ----------------------------------------

void inline ZbGPIO_SwitchLED2(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED_2, Set);
}
// ----------------------------------------

void inline ZbGPIO_ToggleLED1()
{
	ZwGPIO_TogglePin(PIN_LED_1);
}
// ----------------------------------------

void inline ZbGPIO_ToggleLED2()
{
	ZwGPIO_TogglePin(PIN_LED_2);
}
// ----------------------------------------

#endif // __ZBGPIO_H
