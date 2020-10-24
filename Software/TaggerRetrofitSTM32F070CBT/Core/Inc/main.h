/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IR_LED1_Pin GPIO_PIN_0
#define IR_LED1_GPIO_Port GPIOA
#define IR_LED8_Pin GPIO_PIN_1
#define IR_LED8_GPIO_Port GPIOA
#define IR_LED7_Pin GPIO_PIN_2
#define IR_LED7_GPIO_Port GPIOA
#define IR_LED6_Pin GPIO_PIN_3
#define IR_LED6_GPIO_Port GPIOA
#define IR_LED5_Pin GPIO_PIN_4
#define IR_LED5_GPIO_Port GPIOA
#define IR_MOD_Out_Pin GPIO_PIN_6
#define IR_MOD_Out_GPIO_Port GPIOA
#define Button_L_Pin GPIO_PIN_1
#define Button_L_GPIO_Port GPIOB
#define greenLED_Pin GPIO_PIN_11
#define greenLED_GPIO_Port GPIOB
#define PowerCtrl_Pin GPIO_PIN_12
#define PowerCtrl_GPIO_Port GPIOB
#define IR_LED4_Pin GPIO_PIN_13
#define IR_LED4_GPIO_Port GPIOB
#define IR_LED3_Pin GPIO_PIN_14
#define IR_LED3_GPIO_Port GPIOB
#define IR_LED2_Pin GPIO_PIN_15
#define IR_LED2_GPIO_Port GPIOB
#define HeartbeatLED_Pin GPIO_PIN_8
#define HeartbeatLED_GPIO_Port GPIOA
#define RX_Pin GPIO_PIN_12
#define RX_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
