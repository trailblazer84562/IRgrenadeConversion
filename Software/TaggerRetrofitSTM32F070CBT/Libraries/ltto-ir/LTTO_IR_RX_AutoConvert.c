#include "LTTO_IR_RX_AutoConvert.h"

//LTTO_IR_RAW_SIGNATURE_t LTTO_IR_RX_AC_tempRawSignature;
LTTO_IR_SIGNATURE_t LTTO_IR_RX_AC_tempSignature;
LTTO_IR_MULTIBYTE_t LTTO_IR_RX_AC_tempMultibyte;

void LTTO_IR_RX_AC_NewIRPulseReceived(int pulseLengthTicks, bool IRpresentThisPulse) {
    LTTO_IR_RX_NewIRPulseReceived(pulseLengthTicks, IRpresentThisPulse);

    if(LTTO_IR_RX_finalPulseCount) {
        //New LTTO pulse train received...
        //Convert it on up
        //LTTO_IR_CONV_PulsesToRawSignature(LTTO_IR_RX_finalPulseMS, LTTO_IR_RX_finalPulseCount, &LTTO_IR_RX_AC_tempRawSignature);
        LTTO_IR_CONV_RawSignatureToSignature(&LTTO_IR_RX_finalSignature, &LTTO_IR_RX_AC_tempSignature);
        LTTO_IR_RX_finalSignature.valid = false;
        //Check if this is a multibyte...
        if(LTTO_IR_RX_AC_tempSignature.signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE) {
            //Multibyte PType
            //Clear the temp multibyte
            LTTO_IR_CONV_ClearMultibyte(&LTTO_IR_RX_AC_tempMultibyte);
            //Add the new signature
            LTTO_IR_CONV_AddSignatureToMultibyte(&LTTO_IR_RX_AC_tempSignature, &LTTO_IR_RX_AC_tempMultibyte);
        } else if(LTTO_IR_RX_AC_tempSignature.signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA) {
            //Multibyte data
            //Add the new signature
            LTTO_IR_CONV_AddSignatureToMultibyte(&LTTO_IR_RX_AC_tempSignature, &LTTO_IR_RX_AC_tempMultibyte);
        } else if(LTTO_IR_RX_AC_tempSignature.signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM) {
            //Multibyte Checksum
            //Add the new signature
            LTTO_IR_CONV_AddSignatureToMultibyte(&LTTO_IR_RX_AC_tempSignature, &LTTO_IR_RX_AC_tempMultibyte);
            if(!LTTO_IR_RX_AC_tempMultibyte.valid) {
                //Checksum invalid, toss the packet.
                LTTO_IR_CONV_ClearMultibyte(&LTTO_IR_RX_AC_tempMultibyte);
            }
        }
        //And clear the pulse train, so we don't process this signature again.
        LTTO_IR_RX_finalPulseCount = 0;
    }
}

bool LTTO_IR_RX_AC_NewRawSignatureReady(void) {
    return LTTO_IR_RX_finalSignature.valid;
}

void LTTO_IR_RX_AC_GetRawSignature(LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature) {
    destinationRawSignature->headerType = LTTO_IR_RX_finalSignature.headerType;
    destinationRawSignature->bitCount = LTTO_IR_RX_finalSignature.bitCount;
    destinationRawSignature->data = LTTO_IR_RX_finalSignature.data;
    destinationRawSignature->SFP = LTTO_IR_RX_finalSignature.SFP;
    destinationRawSignature->valid = LTTO_IR_RX_finalSignature.valid;
}

bool LTTO_IR_RX_AC_NewSingleSignatureReady(void) {
    return (LTTO_IR_RX_AC_tempSignature.signatureType != LTTO_IR_SIGNATURE_TYPE_UNKNOWN);
}

void LTTO_IR_RX_AC_GetSingleSignature(LTTO_IR_SIGNATURE_t *destinationSignature) {
    destinationSignature->data = LTTO_IR_RX_AC_tempSignature.data;
    destinationSignature->signatureType = LTTO_IR_RX_AC_tempSignature.signatureType;
}

bool LTTO_IR_RX_AC_NewMultibyteReady(void) {
    return LTTO_IR_RX_AC_tempMultibyte.valid;
}

void LTTO_IR_RX_AC_GetMultibyte(LTTO_IR_MULTIBYTE_t *destinationMultibyte) {
    uint8_t temp;
    destinationMultibyte->dataByteCount = LTTO_IR_RX_AC_tempMultibyte.dataByteCount;
    destinationMultibyte->runningChecksum = LTTO_IR_RX_AC_tempMultibyte.runningChecksum;
    destinationMultibyte->valid = LTTO_IR_RX_AC_tempMultibyte.valid;
    for(temp = 0;temp < LTTO_IR_RX_AC_tempMultibyte.dataByteCount;temp++) {
        destinationMultibyte->dataBytes[temp] = LTTO_IR_RX_AC_tempMultibyte.dataBytes[temp];
    }
}

void LTTO_IR_RX_AC_Clear(void) {
    //Clear everything
	LTTO_IR_RX_finalSignature.valid = false;
    LTTO_IR_RX_AC_tempSignature.signatureType = LTTO_IR_SIGNATURE_TYPE_UNKNOWN;
    LTTO_IR_CONV_ClearMultibyte(&LTTO_IR_RX_AC_tempMultibyte);
}
