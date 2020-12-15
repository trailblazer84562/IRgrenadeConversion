#include "LTTO_IR_TX.h"

//Transmitter variables
//Whether or not we're working on sending a signal(Or the delay after a signal)
volatile bool LTTO_IR_TX_currentlyTransmitting = false;
//A timer for keeping track of how long until we need to do the next step of a signal
volatile int LTTO_IR_TX_txDelayTimerMS = 0;  
//Our TX buffer
volatile int LTTO_IR_TX_txPulseMS[21];
volatile int LTTO_IR_TX_txPulseCount = 0;
//And where we're currently at within that buffer
volatile int LTTO_IR_TX_txPulseIndex = 0;
//How long to guarantee between each signal
const int LTTO_IR_TX_txPulseCooldownMS = 75;

void LTTO_IR_TX_Tick1MS(void) {
    if(LTTO_IR_TX_currentlyTransmitting) {
        //If our timer is zero
        if(!LTTO_IR_TX_txDelayTimerMS) {
            //Do some thinking
            if(LTTO_IR_TX_txPulseIndex < LTTO_IR_TX_txPulseCount) {
                if(LTTO_IR_TX_txPulseIndex % 2 == 0) {
                    //Odd index, active IR period
                    LTTO_IR_TX_IREnable();
                    LTTO_IR_TX_txDelayTimerMS = LTTO_IR_TX_txPulseMS[LTTO_IR_TX_txPulseIndex];
                } else {
                    //Even index, inactive IR period
                    LTTO_IR_TX_IRDisable();
                    LTTO_IR_TX_txDelayTimerMS = LTTO_IR_TX_txPulseMS[LTTO_IR_TX_txPulseIndex];
                }
                LTTO_IR_TX_txPulseIndex++;
            } else if(LTTO_IR_TX_txPulseIndex == LTTO_IR_TX_txPulseCount) {
                //End of signal, clean up
                LTTO_IR_TX_IRDisable();
                LTTO_IR_TX_currentlyTransmitting = false;
                LTTO_IR_TX_txPulseIndex = 0;
            }
        }
        if(LTTO_IR_TX_txDelayTimerMS) {
            //Subtract one from the timer
            LTTO_IR_TX_txDelayTimerMS--;
        }
    }
}

void LTTO_IR_TX_QueueSignal(int pulseTickMSBuffer[], int pulseCount) {
    if(!LTTO_IR_TX_currentlyTransmitting) {
    	LTTO_IR_TX_currentlyTransmitting = false;
		LTTO_IR_TX_txPulseIndex = 0;
        for(int i = 0;i < pulseCount;i++) {
            LTTO_IR_TX_txPulseMS[i] = pulseTickMSBuffer[i];
        }
        LTTO_IR_TX_txPulseCount = pulseCount;
        LTTO_IR_TX_currentlyTransmitting = true;
    }
}

void LTTO_IR_TX_QueueRawSignature(LTTO_IR_RAW_SIGNATURE_t *signatureToSend) {
	if(!LTTO_IR_TX_currentlyTransmitting) {
		LTTO_IR_TX_currentlyTransmitting = false;
		LTTO_IR_TX_txPulseIndex = 0;
		LTTO_IR_CONV_RawSignatureToPulses(signatureToSend, &LTTO_IR_TX_txPulseMS, &LTTO_IR_TX_txPulseCount);
		LTTO_IR_TX_currentlyTransmitting = true;
	}
}

void LTTO_IR_TX_QueueSignature(LTTO_IR_SIGNATURE_t *signatureToSend) {
	if(!LTTO_IR_TX_currentlyTransmitting) {
		LTTO_IR_TX_currentlyTransmitting = false;
		LTTO_IR_TX_txPulseIndex = 0;
		LTTO_IR_CONV_SignatureToPulses(signatureToSend, &LTTO_IR_TX_txPulseMS, &LTTO_IR_TX_txPulseCount);
		LTTO_IR_TX_currentlyTransmitting = true;
	}
}

bool LTTO_IR_TX_Busy(void) {
    return LTTO_IR_TX_currentlyTransmitting;
}
