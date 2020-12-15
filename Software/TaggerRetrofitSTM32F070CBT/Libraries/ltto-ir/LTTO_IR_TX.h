#ifndef __LTTO_IR_TX_H__
#define __LTTO_IR_TX_H__

#include <stdint.h>
#include <stdbool.h>

//#include "LTTO_IR_TX_Imp.h"

#include "LTTO_IR_DataStructures.h"
#include "LTTO_IR_Conversions.h"

void LTTO_IR_TX_Tick1MS(void);

void LTTO_IR_TX_QueueSignal(int pulseTickMSBuffer[], int pulseCount);
void LTTO_IR_TX_QueueRawSignature(LTTO_IR_RAW_SIGNATURE_t *signatureToSend);
void LTTO_IR_TX_QueueSignature(LTTO_IR_SIGNATURE_t *signatureToSend);

bool LTTO_IR_TX_Busy(void);

#endif
