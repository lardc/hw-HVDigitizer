// -----------------------------------------
// Auxiliary sampling routines
// ----------------------------------------


// Header
#include "Sampling.h"
//
// Includes
#include "ZwDSP.h"
#include "SysConfig.h"
#include "Global.h"
#include "IQmathUtils.h"

// Definitions
#define TOTAL_SAMPLES	((Int32U)(8 * ADC_LAUNCH_NUM))

// Variables
static Int16U counter = 0;
static Int32S voltage = 0, current = 0;
static _iq adc_Ioffset = 0;

// Functions
//
void SAMPLING_SetChannels(Int16U CurrentIn, Int16U VoltageIn)
{
	Int16U i, ADCChannels[16] = { 0 };

	for (i = 0; i < 16; i += 2)
	{
		ADCChannels[i] = CurrentIn;
		ADCChannels[i + 1] = VoltageIn;
	}

	ZwADC_ConfigureSequentialCascaded(16, ADCChannels);
}
// ----------------------------------------

void SAMPLING_EnableIOffset(Boolean Enable)
{
	adc_Ioffset = (Enable) ? ADC_OFFSET_IQ : 0;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(SAMPLING_LoadData, "ramfuncs");
#endif
Boolean SAMPLING_LoadData(Int16U * const restrict aSampleVector)
{
	Int16U i;

	if (counter < ADC_LAUNCH_NUM)
	{
		counter++;

		#pragma UNROLL(8)
		for(i = 0; i < 16; i += 2)
		{
			current += aSampleVector[i];
			voltage += aSampleVector[i + 1];
		}

		return TRUE;
	}
	else
	{
		counter = 0;
		return FALSE;
	}
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(SAMPLING_DataOutput, "ramfuncs");
#endif
void SAMPLING_DataOutput(_iq *Voltage, _iq *Current)
{
	(*Voltage) = _FP32toIQ2(voltage, TOTAL_SAMPLES) + ADC_OFFSET_IQ;
	(*Current) = _FP32toIQ2(current, TOTAL_SAMPLES) + adc_Ioffset;

	voltage = 0;
	current = 0;
}
// ----------------------------------------

// No more.
