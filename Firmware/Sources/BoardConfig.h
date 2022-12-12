// -----------------------------------------
// Board parameters
// ----------------------------------------

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

// Include
#include <ZwBase.h>
#include "SysConfig.h"

// Program build mode
//
#define BOOT_FROM_FLASH					// normal mode
#define RAM_CACHE_SPI_ABCD				// cache SPI-A(BCD) functions

// Board options
#define OSC_FRQ				(20MHz)			// on-board oscillator
#define CPU_FRQ_MHZ			100				// CPU frequency = 100MHz
#define CPU_FRQ				(CPU_FRQ_MHZ * 1000000L) 
#define SYS_HSP_FREQ		(CPU_FRQ / 2) 	// High-speed bus freq = 50MHz
#define SYS_LSP_FREQ		(CPU_FRQ / 2) 	// Low-speed bus freq = 50MHz
//
#define ZW_PWM_DUTY_BASE	(CPU_FRQ / PWM_FREQ)

// Peripheral options
#define HWUSE_SPI_A
#define HWUSE_SPI_D

// IO placement
#define SPI_A_QSEL		    GPAQSEL2
#define SPI_A_MUX			GPAMUX2
#define SPI_A_SIMO			GPIO16
#define SPI_A_SOMI			GPIO17
#define SPI_A_CLK			GPIO18
#define SPI_A_CS			GPIO19
//
#define SPI_D_QSEL			GPAQSEL1
#define SPI_D_MUX			GPAMUX1
#define SPI_D_SIMO			GPIO1	
#define SPI_D_SOMI			GPIO3		
#define SPI_D_CLK			GPIO5
#define SPI_D_CS			GPIO7
//
#define PIN_LED_1			4
#define PIN_LED_2			28
#define PIN_OE_CS			12
#define PIN_OE_CLK			33
#define PIN_OE_DATA			13

// ADC placement
#define AIN_IH2				0x00	// 0000b = A + IN0
#define AIN_IH				0x01	// 0001b = A + IN1
#define AIN_IL				0x02	// 0010b = A + IN2
//
#define AIN_VL				0x08	// 0100b = B + IN0
#define AIN_VH				0x09	// 0101b = B + IN1
//
#define AIN_LO				0x0D	// 1101b = B + IN5

#endif // __BOARD_CONFIG_H
