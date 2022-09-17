// ----------------------------------------
// Auxiliary sampling routines
// ----------------------------------------

// Header
#include "Sampling.h"
//
// Includes
#include "ZwDSP.h"
#include "SysConfig.h"
#include "Global.h"

// Functions
void SAMPLING_SetChannels(Int16U CurrentIn, Int16U VoltageIn)
{
	Int16U i, ADCChannels[16] = {0};

	for(i = 0; i < 16; i += 2)
	{
		ADCChannels[i] = CurrentIn;
		ADCChannels[i + 1] = VoltageIn;
	}

	ZwADC_ConfigureSequentialCascaded(16, ADCChannels);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(SAMPLING_LoadData, "ramfuncs");
#endif
void SAMPLING_LoadData(Int16U * const restrict aSampleVector, pInt32U Voltage, pInt32U Current)
{
	Int16U i;
	*Voltage = 0;
	*Current = 0;

	#pragma UNROLL(8)
	for(i = 0; i < 16; i += 2)
	{
		*Current += aSampleVector[i];
		*Voltage += aSampleVector[i + 1];
	}

	// Сохраняем один дополнительный бит, полученный в результате оверсэмплинга
	*Voltage >>= 2;
	*Current >>= 2;
}
// ----------------------------------------
