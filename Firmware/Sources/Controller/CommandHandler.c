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
static void CMDH_Commutate(SwitchConfig State);

// Functions
Boolean CMDH_HandleIncomingPacket(pInt16U Buffer)
{
	if ((Buffer[0] >> 8) == IBP_PACKET_START_BYTE)
	{
		switch (Buffer[0] & 0x00FF)
		{
			case IBP_CMD_CFG_SWITCH:
				CMDH_Commutate((SwitchConfig)Buffer[1]);
				break;

			case IBP_CMD_DUMMY:
				Buffer[1] = 0x55;
				Buffer[2] = 0xAA;
				break;

			default:
				break;
		}

		return TRUE;
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

static void CMDH_Commutate(SwitchConfig State)
{
	ZbGPIO_OECompose(OE_MASK_I1 | OE_MASK_I2 | OE_MASK_I3, FALSE);

	switch(State)
	{
		case SwitchConfig_I1:
			ZbGPIO_OECompose(OE_MASK_I1, TRUE);
			break;
		case SwitchConfig_I2:
			ZbGPIO_OECompose(OE_MASK_I2, TRUE);
			break;
		case SwitchConfig_I3:
			ZbGPIO_OECompose(OE_MASK_I3, TRUE);
			break;
	}
}
// ----------------------------------------
