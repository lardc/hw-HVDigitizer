// ----------------------------------------
// Device exchange protocol
// ----------------------------------------

#ifndef __INTER_BOARD_PROTOCOL_H
#define __INTER_BOARD_PROTOCOL_H

// Include
#include "stdinc.h"

// Constants
//
// Protocol
#define IBP_PACKET_START_BYTE		0xA6u
#define IBP_HEADER_SIZE				1
#define IBP_BODY_SIZE				2
#define IBP_PACKET_SIZE				(IBP_HEADER_SIZE + IBP_BODY_SIZE)
#define IBP_CHAR_SIZE 				16
//
// Commands
#define IBP_GET_DATA				0
// 1-4
#define IBP_CMD_CFG_SWITCH			5
#define IBP_CMD_DUMMY				0xFE
#define IBP_ACK						0xFF

// Enums
//
// Switch configuration
typedef enum __SwitchConfig
{
	SwitchConfig_I1 = 0,
	SwitchConfig_I2,
	SwitchConfig_I3
} SwitchConfig;

#endif // __INTER_BOARD_PROTOCOL_H
