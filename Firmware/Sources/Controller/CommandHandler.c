// ----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "CommandHandler.h"
//
// Includes
#include "ZwDSP.h"
#include "ZbBoard.h"
#include "Controller.h"
#include "InterboardProtocol.h"
#include "Global.h"
#include "Sampling.h"

// Forward functions
//
static void CMDH_Sampling(Boolean Enable);
static void CMDH_SetPWM(Int16U Value);
static void CMDH_Commutate(SwitchConfig State);

// Functions
//
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CMDH_HandleIncomingPacket, "ramfuncs");
#endif
Boolean CMDH_HandleIncomingPacket(pInt16U Buffer)
{
	if ((Buffer[0] >> 8) == IBP_PACKET_START_BYTE)
	{
		switch (Buffer[0] & 0x00FF)
		{
			case IBP_CMD_SAMPLING:
				CMDH_Sampling(Buffer[1]);
				return TRUE;

			case IBP_CMD_CFG_SWITCH:
				CMDH_Commutate((SwitchConfig)Buffer[1]);
				return TRUE;

			case IBP_CMD_SET_PWM:
				CMDH_SetPWM(Buffer[1]);
				return TRUE;

			case IBP_CMD_SET_ADC:
				return TRUE;

			default:
				break;
		}
	}
	else
	{
		// Receive all waste clock pulses and re-init SPI
		DELAY_US(1);
		CONTROL_ReInitSPI_Rx();
	}

	return FALSE;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CMDH_SetPWM, "ramfuncs");
#endif
static void CMDH_SetPWM(Int16U Value)
{
	ZwPWM6_SetValueA((Value > ZW_PWM_DUTY_BASE) ? ZW_PWM_DUTY_BASE : Value);
}
// ----------------------------------------

static void CMDH_Commutate(SwitchConfig State)
{
	ZbGPIO_OECompose(OE_MASK_RDC | OE_MASK_R1 | OE_MASK_R2 | OE_MASK_IDC, FALSE);

	switch(State)
	{
		case SwitchConfig_BV:
			ZbGPIO_OECompose(OE_MASK_RDC, TRUE);
			break;
		case SwitchConfig_AC:
			ZbGPIO_OECompose(OE_MASK_R2, TRUE);
			break;
		case SwitchConfig_DC:
			ZbGPIO_OECompose(OE_MASK_R1 | OE_MASK_IDC, TRUE);
			break;
	}
}
// ----------------------------------------

static void CMDH_Sampling(Boolean Enable)
{
	if(Enable)
	{
		ZbGPIO_SwitchLED1(TRUE);
		CONTROL_MeasurementActive = TRUE;
		ZwTimer_StartT0();
	}
	else
	{
		ZbGPIO_SwitchLED1(FALSE);
		CONTROL_MeasurementActive = FALSE;
		ZwTimer_StopT0();
	}
}
// ----------------------------------------
