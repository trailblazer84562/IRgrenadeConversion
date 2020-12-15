#ifndef __LTTO_IR_RX_H__
#define __LTTO_IR_RX_H__

#include <stdint.h>
#include <stdbool.h>

#include "LTTO_IR_RX_Imp.h"

#include "LTTO_IR_DataStructures.h"

//extern volatile int LTTO_IR_RX_finalPulseMS[21];
extern volatile int LTTO_IR_RX_finalPulseCount;
extern volatile LTTO_IR_RAW_SIGNATURE_t LTTO_IR_RX_finalSignature;

void LTTO_IR_RX_NewIRPulseReceived(int pulseLengthTicks, bool IRpresentThisPulse);

#endif
