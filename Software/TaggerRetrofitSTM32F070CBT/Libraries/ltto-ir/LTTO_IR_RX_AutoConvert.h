#ifndef _LTTO_IR_RX_AUTOCONVERT_H__
#define _LTTO_IR_RX_AUTOCONVERT_H__

#include <stdint.h>
#include <stdbool.h>

#include "LTTO_IR_RX.h"
#include "LTTO_IR_DataStructures.h"
#include "LTTO_IR_Conversions.h"

void LTTO_IR_RX_AC_NewIRPulseReceived(int pulseLengthTicks, bool IRpresentThisPulse);

bool LTTO_IR_RX_AC_NewRawSignatureReady(void);
void LTTO_IR_RX_AC_GetRawSignature(LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature);

bool LTTO_IR_RX_AC_NewSingleSignatureReady(void);
void LTTO_IR_RX_AC_GetSingleSignature(LTTO_IR_SIGNATURE_t *destinationSignature);

bool LTTO_IR_RX_AC_NewMultibyteReady(void);
void LTTO_IR_RX_AC_GetMultibyte(LTTO_IR_MULTIBYTE_t *destinationMultibyte);

void LTTO_IR_RX_AC_Clear(void);

#endif