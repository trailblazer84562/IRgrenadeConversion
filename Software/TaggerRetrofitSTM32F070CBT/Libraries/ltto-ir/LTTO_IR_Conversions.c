#include "LTTO_IR_Conversions.h"

void LTTO_IR_CONV_PulsesToRawSignature(int pulseTickMSBuffer[], int pulseCount, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature) {
    //Clear the destination struct.
    destinationRawSignature->valid = false;
    destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_UNKNOWN;
    destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN;
    destinationRawSignature->data = 0;
    destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_UNKNOWN;

    //Need at least 13 pulses to be a valid LTTO signature
    //3 header pulses, and 5 data bits with 2 pulses each.
    if(pulseCount >= 13 && pulseTickMSBuffer[0] == 3 && pulseTickMSBuffer[1] == 6) {
        //Good start...
        if(pulseTickMSBuffer[2] == 3) {
            //Normal header type
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
        } else if(pulseTickMSBuffer[2] == 6) {
            //Beacon header type
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_BEACON;
        }
        {
            int tempBitCount = 0;
            int temp = 0;
            for(temp = 0;temp < ((pulseCount-3)/2);temp++) {
                if(pulseTickMSBuffer[((2*temp)+3)] == 2) {
                    //Valid spacing
                    if(pulseTickMSBuffer[((2*temp)+4)] == 1) {
                        //0 bit
                        //Increment the bit count
                        tempBitCount++;
                        //Shift the data left 1
                        destinationRawSignature->data <<= 1;
                        //And clear the low bit
                        destinationRawSignature->data &= ~(0x0001);
                    } else if(pulseTickMSBuffer[((2*temp)+4)] == 2) {
                        //1 bit
                        //Increment the bit count
                        tempBitCount++;
                        //Shift the data left 1
                        destinationRawSignature->data <<= 1;
                        //And set the low bit
                        destinationRawSignature->data |= 0x0001;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }
            if(tempBitCount == 5) {
                destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5;
            } else if(tempBitCount == 7) {
                destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7;
            } else if(tempBitCount == 8) {
                destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8;
            } else if(tempBitCount == 9) {
                destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
            }
        }

        //Check that the signature is valid
        if(destinationRawSignature->headerType != LTTO_IR_RAW_SIGNATURE_HEADER_UNKNOWN) {
            if(destinationRawSignature->bitCount != LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN) {
                //Can't check the SFP of a received signal
                destinationRawSignature->valid = true;
            }
        }
    }
}

void LTTO_IR_CONV_RawSignatureToPulses(LTTO_IR_RAW_SIGNATURE_t *sourceRawSignature, int destinationPulseTickMSBuffer[], int *destinationPulseCount) {
    if(sourceRawSignature->valid) {
        uint8_t bitCount;
        uint8_t arrayIndex = 0;

        //The basic header
        destinationPulseTickMSBuffer[arrayIndex++] = 3;
        destinationPulseTickMSBuffer[arrayIndex++] = 6;

        //The specific header...
        if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL) {
            destinationPulseTickMSBuffer[arrayIndex++] = 3;
        } else if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_BEACON) {
            destinationPulseTickMSBuffer[arrayIndex++] = 6;
        }

        //Data bits
        switch(sourceRawSignature->bitCount) {
        	default:
        		bitCount = 0;
        		break;
            case LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5:
                bitCount = 5;
                break;
            case LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7:
                bitCount = 7;
                break;
            case LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8:
                bitCount = 8;
                break;
            case LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9:
                bitCount = 9;
                break;
        }
        for(;bitCount != 0;bitCount--) {
            destinationPulseTickMSBuffer[arrayIndex++] = 2;
            if( (sourceRawSignature->data & (0x01 << (bitCount-1))) != 0) {
                destinationPulseTickMSBuffer[arrayIndex++] = 2;
            } else {
                destinationPulseTickMSBuffer[arrayIndex++] = 1;
            }
        }

        //SFP between this and the next packet sent
        switch(sourceRawSignature->SFP) {
            case LTTO_IR_RAW_SIGNATURE_SFP_UNKNOWN:
                //If we don't have one specified, look at the header type to guess at one.
                if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL) {
                    //Generally, normal-headered signatures have a medium SFP
                    destinationPulseTickMSBuffer[arrayIndex++] = 56;
                } else if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_BEACON) {
                    //Generally, beacon-headered signatures have a short SFP
                    destinationPulseTickMSBuffer[arrayIndex++] = 25;
                }
                break;
            case LTTO_IR_RAW_SIGNATURE_SFP_SHORT:
                destinationPulseTickMSBuffer[arrayIndex++] = 25;
                break;
            case LTTO_IR_RAW_SIGNATURE_SFP_MEDIUM:
                destinationPulseTickMSBuffer[arrayIndex++] = 56;
                break;
            case LTTO_IR_RAW_SIGNATURE_SFP_LONG:
                destinationPulseTickMSBuffer[arrayIndex++] = 80;
                break;
        }

        //And save the pulse count
        *destinationPulseCount = arrayIndex;
    }
}

void LTTO_IR_CONV_RawSignatureToSignature(LTTO_IR_RAW_SIGNATURE_t *sourceRawSignature, LTTO_IR_SIGNATURE_t *destinationSignature) {
    //Clear the destination struct
    destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_UNKNOWN;
    destinationSignature->data = 0;

    if(sourceRawSignature->valid) {
        //Start parsing the raw Signature->
        if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL) {
            if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5) {
                //Not known to be valid
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7) {
                //Tag
                destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_TAG;
                destinationSignature->data = sourceRawSignature->data;
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8) {
                //Multibyte data packet
                destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA;
                destinationSignature->data = sourceRawSignature->data;
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9) {
                //Multibyte packet type or checksum
                if( (sourceRawSignature->data & 0x0100) == 0x0100) {
                    //Multibyte Checksum
                    destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM;
                    destinationSignature->data = sourceRawSignature->data;
                } else {
                    //Multibyte Packet Type
                    destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE;
                    destinationSignature->data = sourceRawSignature->data;
                }
            }
        } else if(sourceRawSignature->headerType == LTTO_IR_RAW_SIGNATURE_HEADER_BEACON) {
            if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5) {
                //LTTO beacon
                destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_LTTO_BEACON;
                destinationSignature->data = sourceRawSignature->data;
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7) {
                //Not known to be valid
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8) {
                //Not known to be valid
            } else if(sourceRawSignature->bitCount == LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9) {
                //LTAR beacon
                destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_LTAR_BEACON;
                destinationSignature->data = sourceRawSignature->data;
            }
        }
    }
}

void LTTO_IR_CONV_SignatureToRawSignature(LTTO_IR_SIGNATURE_t *sourceSignature, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature) {
    //Clear the destination struct.
    destinationRawSignature->valid = false;
    destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_UNKNOWN;
    destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN;
    destinationRawSignature->data = 0;
    destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_UNKNOWN;
    
    switch(sourceSignature->signatureType) {
        case LTTO_IR_SIGNATURE_TYPE_LTTO_BEACON:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_BEACON;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5;
            destinationRawSignature->data = sourceSignature->data;
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_SHORT;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_LTAR_BEACON:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_BEACON;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
            destinationRawSignature->data = sourceSignature->data;
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_SHORT;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_TAG:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7;
            destinationRawSignature->data = sourceSignature->data;
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_LONG;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
            destinationRawSignature->data = (sourceSignature->data & ~(0x0100));
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_SHORT;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8;
            destinationRawSignature->data = sourceSignature->data;
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_SHORT;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM:
            destinationRawSignature->headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
            destinationRawSignature->bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
            destinationRawSignature->data = (sourceSignature->data | 0x0100);
            destinationRawSignature->SFP = LTTO_IR_RAW_SIGNATURE_SFP_LONG;
            destinationRawSignature->valid = true;
            break;
        case LTTO_IR_SIGNATURE_TYPE_UNKNOWN:
        	break;
    }
}

void LTTO_IR_CONV_PulsesToSignature(int pulseTickMSBuffer[], int pulseCount, LTTO_IR_SIGNATURE_t *destinationSignature) {
    //Temporary raw Signature->..
    LTTO_IR_RAW_SIGNATURE_t tempRawSignature;
    
    //Convert the pulses to a raw Signature->..
    LTTO_IR_CONV_PulsesToRawSignature(pulseTickMSBuffer, pulseCount, &tempRawSignature);

    LTTO_IR_CONV_RawSignatureToSignature(&tempRawSignature, destinationSignature);
}

void LTTO_IR_CONV_SignatureToPulses(LTTO_IR_SIGNATURE_t *sourceSignature, int destinationPulseTickMSBuffer[], int *destinationPulseCount) {
    LTTO_IR_RAW_SIGNATURE_t tempRawSignature;
    LTTO_IR_CONV_SignatureToRawSignature(sourceSignature, &tempRawSignature);
    LTTO_IR_CONV_RawSignatureToPulses(&tempRawSignature, destinationPulseTickMSBuffer, destinationPulseCount);
}

void LTTO_IR_CONV_MultibyteToSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, LTTO_IR_SIGNATURE_t *destinationSignature) {
    //Clear the destination struct
    destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_UNKNOWN;
    destinationSignature->data = 0;

    if(whichByte < sourceMultibyte->dataByteCount) {
        if(whichByte == 0) {
            destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE;
        } else {
            destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA;
        }
        destinationSignature->data = sourceMultibyte->dataBytes[whichByte];
    }
}

void LTTO_IR_CONV_MultibyteToRawSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature) {
    LTTO_IR_SIGNATURE_t tempSignature;
    LTTO_IR_CONV_MultibyteToSignature(sourceMultibyte, whichByte, &tempSignature);
    LTTO_IR_CONV_SignatureToRawSignature(&tempSignature, destinationRawSignature);
}

void LTTO_IR_CONV_MultibyteToPulses(LTTO_IR_MULTIBYTE_t *sourceMultibyte, uint8_t whichByte, int destinationPulseTickMSBuffer[], int *destinationPulseCount) {
    LTTO_IR_SIGNATURE_t tempSignature;
    LTTO_IR_CONV_MultibyteToSignature(sourceMultibyte, whichByte, &tempSignature);
    LTTO_IR_CONV_SignatureToPulses(&tempSignature, destinationPulseTickMSBuffer, destinationPulseCount);
}

void LTTO_IR_CONV_MultibyteChecksumToSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, LTTO_IR_SIGNATURE_t *destinationSignature) {
    destinationSignature->signatureType = LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM;
    destinationSignature->data = sourceMultibyte->runningChecksum;
}

void LTTO_IR_CONV_MultibyteChecksumToRawSignature(LTTO_IR_MULTIBYTE_t *sourceMultibyte, LTTO_IR_RAW_SIGNATURE_t *destinationRawSignature) {
    LTTO_IR_SIGNATURE_t tempSignature;
    LTTO_IR_CONV_MultibyteChecksumToSignature(sourceMultibyte, &tempSignature);
    LTTO_IR_CONV_SignatureToRawSignature(&tempSignature, destinationRawSignature);
}

void LTTO_IR_CONV_MultibyteChecksumToPulses(LTTO_IR_MULTIBYTE_t *sourceMultibyte, int destinationPulseTickMSBuffer[], int *destinationPulseCount) {
    LTTO_IR_SIGNATURE_t tempSignature;
    LTTO_IR_CONV_MultibyteChecksumToSignature(sourceMultibyte, &tempSignature);
    LTTO_IR_CONV_SignatureToPulses(&tempSignature, destinationPulseTickMSBuffer, destinationPulseCount);
}

void LTTO_IR_CONV_ClearMultibyte(LTTO_IR_MULTIBYTE_t *sourceMultibyte) {
    sourceMultibyte->dataByteCount = 0;
    sourceMultibyte->runningChecksum = 0xFF;
    sourceMultibyte->valid = false;
}

bool LTTO_IR_CONV_AddSignatureToMultibyte(LTTO_IR_SIGNATURE_t *sourceSignature, LTTO_IR_MULTIBYTE_t *destinationMultibyte) {
    bool multibyteOkay = true;
    if(sourceSignature->signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE) {
        if(destinationMultibyte->dataByteCount == 0) {
            destinationMultibyte->dataBytes[0] = sourceSignature->data;
            destinationMultibyte->dataByteCount += 1;
            destinationMultibyte->runningChecksum -= sourceSignature->data;
            destinationMultibyte->valid = false;
        } else {
            multibyteOkay = false;
            destinationMultibyte->dataBytes[0] = sourceSignature->data;
            destinationMultibyte->dataByteCount = 1;
            destinationMultibyte->runningChecksum = (0xFF - sourceSignature->data);
            destinationMultibyte->valid = false;
        }
    } else if(sourceSignature->signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA) {
        if(destinationMultibyte->dataByteCount != 0) {
            destinationMultibyte->dataBytes[destinationMultibyte->dataByteCount++] = sourceSignature->data;
            destinationMultibyte->runningChecksum -= sourceSignature->data;
            destinationMultibyte->valid = false;
        } else {
            multibyteOkay = false;
            destinationMultibyte->valid = false;
        }
    } else if(sourceSignature->signatureType == LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM) {
        if(destinationMultibyte->dataByteCount != 0) {
            if(destinationMultibyte->runningChecksum == sourceSignature->data) {
                destinationMultibyte->valid = true;
            }
        } else {
            multibyteOkay = false;
        }
    }
    return multibyteOkay;
}

void LTTO_IR_CONV_AddDataToMultibyte(uint8_t data, LTTO_IR_MULTIBYTE_t *destinationMultibyte) {
    destinationMultibyte->dataBytes[destinationMultibyte->dataByteCount++] = data;
    destinationMultibyte->runningChecksum -= data;
    destinationMultibyte->valid = true;
}

bool LTTO_IR_CONV_CheckMultibyteComplete(LTTO_IR_MULTIBYTE_t *sourceMultibyte) {
    return sourceMultibyte->valid;
}
