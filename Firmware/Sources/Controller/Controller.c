// -----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
#include "ZwDSP.h"
#include "ZbBoard.h"
#include "Global.h"
#include "InterboardProtocol.h"
#include "Sampling.h"
#include "CommandHandler.h"


// Variables
//
_iq SampledCurrent = 0, SampledVoltage = 0;
//
static CONTROL_FUNC_RealTimeRoutine RealTimeRoutine = NULL;
static Int16U SPIInputDataBuffer[IBP_PACKET_SIZE];
volatile Boolean CONTROL_MeasurementActive = FALSE;
static volatile FUNC_AsyncDelegate DPCDelegate = NULL;


// Forward functions
//
static Boolean CONTROL_ReceiveData();
static void CONTROL_SendData(pInt16U Buffer);
static void CONTROL_ConversionDone(Int16U * const restrict aSampleVector);
void CONTROL_HandleTransmission();


// Functions
//
void CONTROL_Init()
{
	ZwADC_SubscribeToResults1(&CONTROL_ConversionDone);

	if(ZwSystem_GetDogAlarmFlag())
		ZwSystem_ClearDogAlarmFlag();

	// Set switches to default state
	ZbGPIO_OEFlush(TRUE);
	DELAY_US(1000);
}
// ----------------------------------------

void CONTROL_Idle()
{
	// Handle external register change
	ZbGPIO_OEFlush(FALSE);

	// Process deferred procedures
	if(DPCDelegate)
	{
		FUNC_AsyncDelegate del = DPCDelegate;
		DPCDelegate = NULL;
		del();
	}
}
// ----------------------------------------

void CONTROL_RequestDPC(FUNC_AsyncDelegate Action)
{
	DPCDelegate = Action;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_HandleTransmission, "ramfuncs");
#endif
void CONTROL_HandleTransmission()
{
	Int16U SPIBuffer[IBP_PACKET_SIZE] = { 0 };

	// Receive master data
	if(CONTROL_ReceiveData())
	{
		// Handle incoming packet
		if (SPIInputDataBuffer[0] == ((IBP_PACKET_START_BYTE << 8) | IBP_GET_DATA))
		{
			SPIBuffer[0] = SPIInputDataBuffer[0];

			// Format measurement data
			// | Current (24 bit)			|	Voltage (24 bit)		|
			// ||		16		||		8	|	8		||		16		||
			if (CONTROL_MeasurementActive)
			{
				SPIBuffer[1] = ((Int32U)SampledCurrent) >> 16;
				SPIBuffer[2] = ((Int32U)SampledCurrent) & 0xFF00;
				SPIBuffer[2] |= ((Int32U)SampledVoltage) >> 24;
				SPIBuffer[3] = ((Int32U)SampledVoltage) >> 8;
			}

			// Send data to master
			CONTROL_SendData(SPIBuffer);
		}
		else
		{
			// Handle CMD packet
			if (CMDH_HandleIncomingPacket(SPIInputDataBuffer))
			{
				SPIBuffer[0] = (Int16U)((IBP_PACKET_START_BYTE << 8) | IBP_ACK);

				// Format measurement data
				if(CONTROL_MeasurementActive)
				{
					SPIBuffer[1] = ((Int32U)SampledCurrent) >> 16;
					SPIBuffer[2] = ((Int32U)SampledCurrent) & 0xFF00;
					SPIBuffer[2] |= ((Int32U)SampledVoltage) >> 24;
					SPIBuffer[3] = ((Int32U)SampledVoltage) >> 8;
				}

				// Send data to master
				CONTROL_SendData(SPIBuffer);
			}
		}
	}
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_SendData, "ramfuncs");
#endif
static void CONTROL_SendData(pInt16U Buffer)
{
	ZwSPId_Send(Buffer, IBP_PACKET_SIZE, IBP_CHAR_SIZE, STTStream);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_UpdateHigh, "ramfuncs");
#endif
void CONTROL_UpdateHigh()
{
	if(CONTROL_MeasurementActive)
		ZwADC_StartSEQ1();

	if (RealTimeRoutine)
		RealTimeRoutine();
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_SubcribeToCycle, "ramfuncs");
#endif
void CONTROL_SubcribeToCycle(CONTROL_FUNC_RealTimeRoutine Routine)
{
	RealTimeRoutine = Routine;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_ReceiveData, "ramfuncs");
#endif
static Boolean CONTROL_ReceiveData()
{
	if(ZwSPIa_GetWordsToReceive() >= IBP_PACKET_SIZE)
	{
		ZwSPIa_EndReceive(SPIInputDataBuffer, IBP_PACKET_SIZE);
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_ConversionDone, "ramfuncs");
#endif
static void CONTROL_ConversionDone(Int16U * const restrict aSampleVector)
{
	// Convert sampled data
	if (SAMPLING_LoadData(aSampleVector))
		ZwADC_StartSEQ1();
	else
		SAMPLING_DataOutput(&SampledVoltage, &SampledCurrent);
}
// ----------------------------------------

void CONTROL_ReInitSPI_Rx()
{
	// Init master Rx optical interface
	ZwSPIa_Init(FALSE, 0, IBP_CHAR_SIZE, SPIA_PLR, SPIA_PHASE, ZW_SPI_INIT_RX, FALSE, FALSE);
}
// -----------------------------------------

// No more.
