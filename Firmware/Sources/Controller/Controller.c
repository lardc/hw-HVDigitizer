// ----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"

// Includes
#include "SysConfig.h"
#include "ZwDSP.h"
#include "ZbBoard.h"
#include "Global.h"
#include "InterboardProtocol.h"
#include "Sampling.h"
#include "CommandHandler.h"
#include "BoardConfig.h"

// Variables
static volatile Int16U SPIInputDataBuffer[IBP_PACKET_SIZE], SPIOutputDataBuffer[IBP_PACKET_SIZE];
static volatile Boolean SampleActive = FALSE;
volatile Int64U CONTROL_TimeCounter, CONTROL_InterfaceLED;

// Forward functions
static Boolean CONTROL_ReceiveData();
static void CONTROL_SendData();
static void CONTROL_ConversionDone(Int16U * const restrict aSampleVector);
void CONTROL_HandleTransmission();


// Functions
void CONTROL_Init()
{
	ZwADC_SubscribeToResults1(&CONTROL_ConversionDone);

	if(ZwSystem_GetDogAlarmFlag())
		ZwSystem_ClearDogAlarmFlag();

	SAMPLING_SetChannels(AIN_IH, AIN_VH);
	ZbGPIO_OEFlush(TRUE);
	DELAY_US(1000);
}
// ----------------------------------------

void CONTROL_Idle()
{
	// Handle external register change
	ZbGPIO_OEFlush(FALSE);

	// Гашение светодиода
	if(CONTROL_TimeCounter > CONTROL_InterfaceLED)
		ZbGPIO_SwitchLED1(FALSE);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_HandleTransmission, "ramfuncs");
#endif
void CONTROL_HandleTransmission()
{
	// Receive master data
	if(CONTROL_ReceiveData())
	{
		CONTROL_InterfaceLED = CONTROL_TimeCounter + SMPL_LED_BLINK;
		ZbGPIO_SwitchLED1(TRUE);

		// Запуск оцифровки
		if(SPIInputDataBuffer[0] == ((IBP_PACKET_START_BYTE << 8) | IBP_GET_DATA))
		{
			if(!SampleActive)
			{
				SampleActive = TRUE;
				ZwADC_StartSEQ1();
			}
		}

		// Обработка пакета с командой
		else if(CMDH_HandleIncomingPacket((pInt16U)SPIInputDataBuffer))
		{
			SPIInputDataBuffer[0] = (Int16U)((IBP_PACKET_START_BYTE << 8) | IBP_ACK);
			CONTROL_SendData(SPIInputDataBuffer);
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
	#pragma CODE_SECTION(CONTROL_ReceiveData, "ramfuncs");
#endif
static Boolean CONTROL_ReceiveData()
{
	if(ZwSPIa_GetWordsToReceive() >= IBP_PACKET_SIZE)
	{
		ZwSPIa_EndReceive((pInt16U)SPIInputDataBuffer, IBP_PACKET_SIZE);
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
	Int32U Voltage, Current;
	SAMPLING_LoadData(aSampleVector, &Voltage, &Current);
	SampleActive = FALSE;

	// Упаковка и отправка значений
	SPIOutputDataBuffer[0] = (IBP_PACKET_START_BYTE << 8) | IBP_GET_DATA;
	SPIOutputDataBuffer[1] = Voltage;
	SPIOutputDataBuffer[2] = Current;

	CONTROL_SendData((pInt16U)SPIOutputDataBuffer);
}
// ----------------------------------------

void CONTROL_ReInitSPI_Rx()
{
	// Init master Rx optical interface
	ZwSPIa_Init(FALSE, 0, IBP_CHAR_SIZE, SPIA_PLR, SPIA_PHASE, ZW_SPI_INIT_RX, FALSE, FALSE);
}
// ----------------------------------------
