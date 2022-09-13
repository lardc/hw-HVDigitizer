// ----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

// Header
//
#include "ZbGPIO.h"

// Variable
//
static volatile Int16U Register = 0;

// Functions
//
void ZbGPIO_Init()
{
	// Reset to default state
	ZwGPIO_WritePin(PIN_LED_1, FALSE);
	ZwGPIO_WritePin(PIN_LED_2, FALSE);
	//
	ZwGPIO_WritePin(PIN_OE_CS, TRUE);
	ZwGPIO_WritePin(PIN_OE_CLK, TRUE);
	ZwGPIO_WritePin(PIN_OE_DATA, FALSE);

	// Configure pins
	ZwGPIO_PinToOutput(PIN_LED_1);
	ZwGPIO_PinToOutput(PIN_LED_2);
	//
		ZwGPIO_PinToOutput(PIN_OE_CS);
		ZwGPIO_PinToOutput(PIN_OE_CLK);
		ZwGPIO_PinToOutput(PIN_OE_DATA);
}
// ----------------------------------------

void ZbGPIO_OECompose(Int16U Mask, Boolean Enable)
{
	if(Enable)
		Register |= Mask;
	else
		Register &= ~Mask;
}
// ----------------------------------------

void ZbGPIO_OEFlush(Boolean Force)
{
	Int16U i;
	static Int16U PrevRegister = 0;

	if(PrevRegister != Register || Force)
	{
		PrevRegister = Register;

		ZwGPIO_WritePin(PIN_OE_CS, FALSE);
		DELAY_US(1);

		for(i = 8; i > 0; --i)
		{
			ZwGPIO_WritePin(PIN_OE_CLK, FALSE);
			ZwGPIO_WritePin(PIN_OE_DATA, PrevRegister & (1 << (i - 1)));
			DELAY_US(1);
			ZwGPIO_WritePin(PIN_OE_CLK, TRUE);
			DELAY_US(1);
		}

		ZwGPIO_WritePin(PIN_OE_CS, TRUE);
	}
}
// ----------------------------------------
