#ifndef __LTTO_IR_TX_IMP_H__
#define __LTTO_IR_TX_IMP_H__

#include <stdint.h>
#include <stdbool.h>

//#define LTTO_IR_TX_IREnable()
static inline void LTTO_IR_TX_IREnable(void) {
	uint32_t tempPinMask = 0;
	if(LTTO_IR_TX_LED_Enable3and4) {
		tempPinMask |= 0x0002;
	}
	if(LTTO_IR_TX_LED_Enable1and2) {
		tempPinMask |= 0x0004;
	}
	if(LTTO_IR_TX_LED_Enable5and7) {
		tempPinMask |= 0x0008;
	}
	if(LTTO_IR_TX_LED_Enable6and8) {
		tempPinMask |= 0x0010;
	}
	STM_GPIO_SetPinState(GPIO_PORTA, tempPinMask, true);
}

//#define LTTO_IR_TX_IRDisable()
static inline void LTTO_IR_TX_IRDisable(void) {
	//Turn off all the LEDs
	HAL_GPIO_WritePin(IR_LED1_GPIO_Port, IR_LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED2_GPIO_Port, IR_LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED3_GPIO_Port, IR_LED3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED4_GPIO_Port, IR_LED4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED5_GPIO_Port, IR_LED5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED6_GPIO_Port, IR_LED6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED7_GPIO_Port, IR_LED7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR_LED8_GPIO_Port, IR_LED8_Pin, GPIO_PIN_RESET);
}

#endif
