#ifndef __LTTO_IR_CONVERSIONS_H__
#define __LTTO_IR_CONVERSIONS_H__

#include <stdint.h>
#include <stdbool.h>

#include "LTTO_IR_DataStructures.h"

void LTTO_IR_CONV_PulsesToRawSignature(int pulseTickMSBuffer[], int pulseCount, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature);
void LTTO_IR_CONV_RawSignatureToPulses(LTTO_IR_RAW_SIGNATURE_t *sourceSignature, int destinationPulseTickMSBuffer[], int *destinationPulseCount);

void LTTO_IR_CONV_RawSignatureToSignature(LTTO_IR_RAW_SIGNATURE_t *sourceRawSignature, LTTO_IR_SIGNATURE_t *destinationSignature);
void LTTO_IR_CONV_SignatureToRawSignature(LTTO_IR_SIGNATURE_t *sourceSignature, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature);

void LTTO_IR_CONV_PulsesToSignature(int pulseTickMSBuffer[], int pulseCount, LTTO_IR_SIGNATURE_t *destinationSignature);
void LTTO_IR_CONV_SignatureToPulses(LTTO_IR_SIGNATURE_t *sourceSignature, int destinationPulseTickMSBuffer[], int *destinationPulseCount);

void LTTO_IR_CONV_MultibyteToSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, LTTO_IR_SIGNATURE_t *destinationSignature);
void LTTO_IR_CONV_MultibyteToRawSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature);
void LTTO_IR_CONV_MultibyteToPulses(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, int destinationPulseTickMSBuffer[], int *destinationPulseCount);

void LTTO_IR_CONV_MultibyteChecksumToSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, LTTO_IR_SIGNATURE_t *destinationSignature);
void LTTO_IR_CONV_MultibyteChecksumToRawSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature);
void LTTO_IR_CONV_MultibyteChecksumToPulses(LTTO_IR_MULTIBYTE_t *sourceMultibyte, int destinationPulseTickMSBuffer[], int *destinationPulseCount);

void LTTO_IR_CONV_ClearMultibyte(LTTO_IR_MULTIBYTE_t *sourceMultibyte);

bool LTTO_IR_CONV_AddSignatureToMultibyte(LTTO_IR_SIGNATURE_t *sourceSignature, LTTO_IR_MULTIBYTE_t *destinationMultibyte);
void LTTO_IR_CONV_AddDataToMultibyte(uint8_t data, LTTO_IR_MULTIBYTE_t *destinationMultibyte);

bool LTTO_IR_CONV_CheckMultibyteComplete(LTTO_IR_MULTIBYTE_t *sourceMultibyte);

#endif
