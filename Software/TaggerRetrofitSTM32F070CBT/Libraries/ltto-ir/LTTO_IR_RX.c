#include "LTTO_IR_RX.h"

//LTTO decoding variables
//volatile int LTTO_IR_RX_WIPpulseMS[21];
volatile int LTTO_IR_RX_WIPpulseCount = 0;

//Finished received LTTO signal variables
//volatile int LTTO_IR_RX_finalPulseMS[21];
volatile int LTTO_IR_RX_finalPulseCount = 0;

volatile LTTO_IR_RAW_SIGNATURE_t LTTO_IR_RX_wipSignature;
volatile LTTO_IR_RAW_SIGNATURE_t LTTO_IR_RX_finalSignature;

void LTTO_IR_RX_NewIRPulseReceived(int pulseLengthTicks, bool IRpresentThisPulse) {
    switch(LTTO_IR_RX_WIPpulseCount) {
        case 0:
            //Looking for a ~3ms pre-sync pulse
            if(IRpresentThisPulse && (pulseLengthTicks > (2.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (3.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[0] = 3;
                LTTO_IR_RX_WIPpulseCount++;
            }
            break;
        case 1:
            //Looking for a ~6ms pre-sync pause
            if(!IRpresentThisPulse && (pulseLengthTicks > (5.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (7.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[1] = 6;
                LTTO_IR_RX_WIPpulseCount++;
            } else {
                //Not what we're looking for.
                //Abort!
                LTTO_IR_RX_WIPpulseCount = 0;
            }
            break;
        case 2:
            //Looking for either a ~3ms Tag/Data Sync Pulse, or a ~6ms Beacon Sync Pulse.
            if(IRpresentThisPulse && (pulseLengthTicks > (2.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (3.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[2] = 3;
            	LTTO_IR_RX_wipSignature.headerType = LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL;
                LTTO_IR_RX_WIPpulseCount++;
            } else if(IRpresentThisPulse && (pulseLengthTicks > (5.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (6.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[2] = 6;
            	LTTO_IR_RX_wipSignature.headerType = LTTO_IR_RAW_SIGNATURE_HEADER_BEACON;
                LTTO_IR_RX_WIPpulseCount++;
            } else {
                //Not what we're looking for.
                //Abort!
                LTTO_IR_RX_WIPpulseCount = 0;
            }
            break;
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 15:
        case 17:
        case 19:
            //Looking for a ~2ms data pause or something longer than ~2.5ms as a sign that either we're done or something got corrupted.
            if(!IRpresentThisPulse && (pulseLengthTicks > (1.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (2.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[LTTO_IR_RX_WIPpulseCount] = 2;
                LTTO_IR_RX_WIPpulseCount++;
            } else if(!IRpresentThisPulse && (pulseLengthTicks > (2.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //We're done!
                //Copy things to the "forground" buffer, so that the gameplay loop can s  ee we got something.
                /*for(int i = 0;i < LTTO_IR_RX_WIPpulseCount;i++) {
                    LTTO_IR_RX_finalPulseMS[i] = LTTO_IR_RX_WIPpulseMS[i];
                }*/
            	uint8_t temp = ((LTTO_IR_RX_WIPpulseCount - 3) / 2);
            	LTTO_IR_RX_finalSignature.headerType = LTTO_IR_RX_wipSignature.headerType;
            	LTTO_IR_RX_finalSignature.data = LTTO_IR_RX_wipSignature.data;
            	//LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RX_wipSignature.bitCount;
            	switch(temp) {
					default:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN;
						break;
					case 5:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5;
						break;
					case 7:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7;
						break;
					case 8:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8;
						break;
					case 9:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
						break;
            	}
            	LTTO_IR_RX_finalSignature.SFP = LTTO_IR_RX_wipSignature.SFP;
            	if(LTTO_IR_RX_finalSignature.bitCount != LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN) {
					LTTO_IR_RX_finalSignature.valid = true;
					LTTO_IR_RX_finalPulseCount = LTTO_IR_RX_WIPpulseCount;
            	}
                //And reset the receiver counter.
                LTTO_IR_RX_WIPpulseCount = 0;
            } else {
                //Not what we're looking for. Abort!
                LTTO_IR_RX_WIPpulseCount = 0;
            }
            break;
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
        case 14:
        case 16:
        case 18:
        case 20:
            //Looking for either a ~1ms "0" data bit, or a ~2ms "1" data bit.
            if(IRpresentThisPulse && (pulseLengthTicks > (0.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (1.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //LTTO_IR_RX_WIPpulseMS[LTTO_IR_RX_WIPpulseCount] = 1;
            	LTTO_IR_RX_wipSignature.data <<= 1;
            	LTTO_IR_RX_wipSignature.data &= ~(0x0001);
                LTTO_IR_RX_WIPpulseCount++;
            } else if(IRpresentThisPulse && (pulseLengthTicks > (1.5*LTTO_IR_RX_TICKS_PER_MS)) && (pulseLengthTicks < (2.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
            	LTTO_IR_RX_wipSignature.data <<= 1;
				LTTO_IR_RX_wipSignature.data |= 0x0001;
                LTTO_IR_RX_WIPpulseCount++;
            } else {
                //Not what we're looking for.
                //Abort!
                LTTO_IR_RX_WIPpulseCount = 0;
            }
            break;
        case 21:
            //Looking for a period of silence, at least 2.5ms.
            if(!IRpresentThisPulse && (pulseLengthTicks > (2.5*LTTO_IR_RX_TICKS_PER_MS)) ) {
                //We're done!
            	//Copy things to the "forground" buffer, so that the gameplay loop can s  ee we got something.
				/*for(int i = 0;i < LTTO_IR_RX_WIPpulseCount;i++) {
					LTTO_IR_RX_finalPulseMS[i] = LTTO_IR_RX_WIPpulseMS[i];
				}*/
				uint8_t temp = ((LTTO_IR_RX_WIPpulseCount - 3) / 2);
				LTTO_IR_RX_finalSignature.headerType = LTTO_IR_RX_wipSignature.headerType;
				LTTO_IR_RX_finalSignature.data = LTTO_IR_RX_wipSignature.data;
				//LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RX_wipSignature.bitCount;
				switch(temp) {
					default:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN;
						break;
					case 5:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5;
						break;
					case 7:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7;
						break;
					case 8:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8;
						break;
					case 9:
						LTTO_IR_RX_finalSignature.bitCount = LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9;
						break;
				}
				LTTO_IR_RX_finalSignature.SFP = LTTO_IR_RX_wipSignature.SFP;
				if(LTTO_IR_RX_finalSignature.bitCount != LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN) {
					LTTO_IR_RX_finalSignature.valid = true;
					LTTO_IR_RX_finalPulseCount = LTTO_IR_RX_WIPpulseCount;
				}
				//And reset the receiver counter.
				LTTO_IR_RX_WIPpulseCount = 0;
            } else {
                //Something's wrong. Nothing in the LTTO protocol is this long.
                //Abort!
                LTTO_IR_RX_WIPpulseCount = 0;
            }
            break;
    }
}
