// ----------------------------------------
// System parameters
// ----------------------------------------

#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

// Include
#include <ZwBase.h>
#include <BoardConfig.h>

// CPU & System
//--------------------------------------------------------
#define CPU_PLL				10			// OSCCLK * PLL div 2 = CPUCLK: 20 * 10 / 2 = 100
#define CPU_CLKINDIV		0			// "div 2" in previous equation
#define SYS_HISPCP			0x01		// SYSCLKOUT / 2
#define SYS_LOSPCP			0x01		// SYSCLKOUT / 2
//--------------------------------------------------------

// Power control
//--------------------------------------------------------
#define SYS_PUMOD			ZW_POWER_ADC_CLK  | ZW_POWER_SPIA_CLK | \
							ZW_POWER_SPID_CLK

#define SYS_WD_PRESCALER	0x07
//--------------------------------------------------------

// GPIO
//--------------------------------------------------------
// Input filters
#define GPIO_TSAMPLE		10			// T[sample_A] = (1/ 100MHz) * (2 * 10) = 200 nS
//--------------------------------------------------------

// Flash
//--------------------------------------------------------
#define FLASH_FWAIT			3
#define FLASH_OTPWAIT		5
//--------------------------------------------------------

// TIMERs
//--------------------------------------------------------
#define CS_T2_FREQ			1000		// 1 kHz
#define TIMER2_PERIOD		(1000000L / CS_T2_FREQ)

#define DBG_FREQ			2			// 2 Hz
#define DBG_COUNTER_PERIOD	(CS_T2_FREQ / (DBG_FREQ * 2))
//--------------------------------------------------------

// SPI-A
//--------------------------------------------------------
#define SPIA_PLR			TRUE		// CLK high in idle state
#define SPIA_PHASE			FALSE
//--------------------------------------------------------

// SPI-D
//--------------------------------------------------------
#define SPID_BAUDRATE		4000000L	// SPI clock in MHz
#define SPID_PLR			TRUE		// CLK high in idle state
#define SPID_PHASE			FALSE
//--------------------------------------------------------

// ADC
//--------------------------------------------------------
#define ADC_PRESCALER		0			// HSPCLK / (1 * 1) = 50 MHz
#define ADC_CD2				TRUE		// Divide ADC core clock / 2 = 25MHz
#define ADC_SH				1			// S/H sample window = 2 => 12.5 MSPS
//--------------------------------------------------------

// MISC
//--------------------------------------------------------
#define MSC_PON_DELAY_MS	500			// 500 ms
#define MSC_PON_SPI_RST		1500		// 1500 ms
//--------------------------------------------------------

#endif // __SYSCONFIG_H
