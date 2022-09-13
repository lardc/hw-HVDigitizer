// ----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CMD_HANDLER_H
#define __CMD_HANDLER_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"

// Functions
//
// Dispatch control packet
Boolean CMDH_HandleIncomingPacket(pInt16U Buffer);

#endif // __CMD_HANDLER_H
