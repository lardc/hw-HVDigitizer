// ----------------------------------------
// Program entry point
// ----------------------------------------

// Include
#include <stdinc.h>
//
#include "ZwDSP.h"
#include "ZbBoard.h"
//
#include "SysConfig.h"
//
#include "Controller.h"
#include "InterboardProtocol.h"
#include "Global.h"

// FORWARD FUNCTIONS
// ----------------------------------------
Boolean InitializeCPU();
void InitializeTimers();
void InitializeADC();
void InitializeSPI();
void InitializeBoard();
void InitializeController();
// ----------------------------------------

// FORWARD ISRs
// ----------------------------------------
// CPU Timer 0 ISR
ISRCALL Timer0_ISR();
// CPU Timer 2 ISR
ISRCALL Timer2_ISR();
// ADC SEQ1 ISR
ISRCALL SEQ1_ISR();
// SPI-A RX ISR
ISRCALL SPIaRX_ISR();
// ILLEGAL ISR
ISRCALL IllegalInstruction_ISR();
// ----------------------------------------

// FUNCTIONS
// ----------------------------------------
// Program main function
void main()
{
	// Boot process
	InitializeCPU();

	// Switch GPIO in proper state
	InitializeBoard();

	// Wait for power-on
	Int16U i;
	for(i = 0; i < MSC_PON_DELAY_MS; ++i) DELAY_US(1000);

	// Only if good clocking was established
	InitializeTimers();
	InitializeADC();
	InitializeSPI();

	// Setup ISRs
	BEGIN_ISR_MAP
		ADD_ISR(TINT0, Timer0_ISR);
		ADD_ISR(TINT2, Timer2_ISR);
		ADD_ISR(SEQ1INT, SEQ1_ISR);
		ADD_ISR(SPIRXINTA, SPIaRX_ISR);
		ADD_ISR(ILLEGAL, IllegalInstruction_ISR);
	END_ISR_MAP

	// Initialize controller logic
	InitializeController();

	// Enable interrupts
	EINT;
	ERTM;

	// Set watch-dog as WDRST
	ZwSystem_SelectDogFunc(FALSE);
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
	ZwSystem_LockDog();

	// Start timers
	ZwTimer_StartT2();

	// Re-init SPI
	DELAY_US(MSC_PON_SPI_RST * 1000);
	CONTROL_ReInitSPI_Rx();

	// Background cycle
	while(TRUE)
		CONTROL_Idle();
}
// ----------------------------------------

// Initialize and prepare DSP
Boolean InitializeCPU()
{
	Boolean clockInitResult;

	// Init clock and peripherals
	clockInitResult = ZwSystem_Init(CPU_PLL, CPU_CLKINDIV, SYS_LOSPCP, SYS_HISPCP, SYS_PUMOD);

	if(clockInitResult)
	{
		// Do default GPIO configuration
		ZwGPIO_Init(GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE);
		// Initialize PIE
		ZwPIE_Init();
		// Prepare PIE vectors
		ZwPIE_Prepare();
	}

	// Configure flash
	ZW_FLASH_CODE_SHADOW;
	ZW_FLASH_MATH_SHADOW;
	ZW_FLASH_OPTIMIZE(FLASH_FWAIT, FLASH_OTPWAIT);

	return clockInitResult;
}
// ----------------------------------------

// Initialize CPU timers
void InitializeTimers()
{
	ZwTimer_InitT0();
	ZwTimer_SetT0(TIMER0_PERIOD);
	ZwTimer_EnableInterruptsT0(TRUE);

	ZwTimer_InitT2();
	ZwTimer_SetT2(TIMER2_PERIOD);
	ZwTimer_EnableInterruptsT2(TRUE);
}
// ----------------------------------------

void InitializeADC()
{
	// Initialize and prepare ADC
	ZwADC_Init(ADC_PRESCALER, ADC_CD2, ADC_SH);
	ZwADC_ConfigInterrupts(TRUE, FALSE);

	// Enable interrupts on peripheral and CPU levels
	ZwADC_EnableInterrupts(TRUE, FALSE);
	ZwADC_EnableInterruptsGlobal(TRUE);
}
// ----------------------------------------

void InitializeSPI()
{
	// Tx to master
	ZwSPId_Init(TRUE, SPID_BAUDRATE, IBP_CHAR_SIZE, SPID_PLR, SPID_PHASE, ZW_SPI_INIT_TX, FALSE, FALSE);
	ZwSPId_InitFIFO(0, 0);
	ZwSPId_ConfigInterrupts(FALSE, FALSE);
	ZwSPId_EnableInterrupts(FALSE, FALSE);

	// Rx from master
	ZwSPIa_Init(FALSE, 0, IBP_CHAR_SIZE, SPIA_PLR, SPIA_PHASE, ZW_SPI_INIT_RX, FALSE, FALSE);
	ZwSPIa_InitFIFO(IBP_PACKET_SIZE, 0);
	ZwSPIa_ConfigInterrupts(TRUE, FALSE);
	ZwSPIa_EnableInterrupts(TRUE, FALSE);

	// Common (ABCD)
	ZwSPI_EnableInterruptsGlobal(TRUE);
}
// ----------------------------------------

void InitializeBoard()
{
	ZbGPIO_Init();
}
// ----------------------------------------

void InitializeController()
{
	CONTROL_Init();
}
// ----------------------------------------

// ISRs
// ----------------------------------------
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(Timer0_ISR, "ramfuncs");
	#pragma CODE_SECTION(Timer2_ISR, "ramfuncs");
	#pragma CODE_SECTION(SEQ1_ISR, "ramfuncs");
	#pragma CODE_SECTION(SPIaRX_ISR, "ramfuncs");
	#pragma CODE_SECTION(IllegalInstruction_ISR, "ramfuncs");
#endif
//
#pragma INTERRUPT(Timer0_ISR, HPI);
#pragma INTERRUPT(SEQ1_ISR, HPI);

ISRCALL Timer0_ISR()
{
	CONTROL_UpdateHigh();

	// allow other interrupts from group 1
	TIMER0_ISR_DONE;
}
// ----------------------------------------

ISRCALL Timer2_ISR()
{
	static Int16U dbgCounter = 0;

	// Service watch-dogs
	ZwSystem_ServiceDog();

	// Blink LED
	++dbgCounter;
	if(dbgCounter == DBG_COUNTER_PERIOD)
	{
		ZbGPIO_ToggleLED2();
		dbgCounter = 0;
	}

	// no PIE
	TIMER2_ISR_DONE;
}
// ----------------------------------------

// ADC SEQ1 ISR
ISRCALL SEQ1_ISR()
{
	// Handle interrupt
	ZwADC_ProcessInterruptSEQ1();
	// Dispatch results
	ZwADC_Dispatch1();

	// allow other interrupts from group 1
	ADC_ISR_DONE;
}
// ----------------------------------------

// SPI-A RX ISR
ISRCALL SPIaRX_ISR()
{
	// Handle interrupt
	ZwSPIa_ProcessRXInterrupt();
	// Handle master packet
	CONTROL_HandleTransmission();

	// allow other interrupts from group 6
	SPI_ISR_DONE;
}
// ----------------------------------------

// ILLEGAL ISR
ISRCALL IllegalInstruction_ISR()
{
	// Disable interrupts
	DINT;

	// Reset system using WD
	ZwSystem_ForceDog();
}
// ----------------------------------------
