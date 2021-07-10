/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "queue.h"
#include "stdbool.h"

#include "../../Libraries/ltto-ir/LTTO_IR_DataStructures.h"
#include "../../Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.h"
#include "../../Libraries/ltto-ir/LTTO_IR_RX.h"
#include "../../Libraries/ltto-ir/LTTO_IR_TX.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	PROGRAM = 1,
	COUNTDOWN = 2,
	DETONATE = 3,
	SHUTDOWN = 4
} StateMachineState;

typedef enum {
	ULTRA = 1,
	SLOW_BLINK = 2,
	MEDIUM_BLINK = 3,
	FAST_BLINK = 4,
	SOLID_LED = 5,
	OCCASIONAL_BLIP = 6
} LED_BlinkProfile;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for emitTag */
osThreadId_t emitTagHandle;
const osThreadAttr_t emitTag_attributes = {
  .name = "emitTag",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 128 * 4
};
/* Definitions for LED */
osThreadId_t LEDHandle;
const osThreadAttr_t LED_attributes = {
  .name = "LED",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for buttonMonitor */
osThreadId_t buttonMonitorHandle;
const osThreadAttr_t buttonMonitor_attributes = {
  .name = "buttonMonitor",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for LED_BlinkStyle */
osMessageQueueId_t LED_BlinkStyleHandle;
const osMessageQueueAttr_t LED_BlinkStyle_attributes = {
  .name = "LED_BlinkStyle"
};
/* Definitions for tagSignature */
osMessageQueueId_t tagSignatureHandle;
const osMessageQueueAttr_t tagSignature_attributes = {
  .name = "tagSignature"
};
/* USER CODE BEGIN PV */
StateMachineState presentState = PROGRAM;
LTTO_IR_SIGNATURE_t tempSignature;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM6_Init(void);
void StartDefaultTask(void *argument);
void StartEmitTag(void *argument);
void StartLEDFlash(void *argument);
void StartButtonMonitor(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

	//Start Timer
	HAL_TIM_Base_Start_IT(&htim6);
	// Start the 38kHz PWM output
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of LED_BlinkStyle */
  LED_BlinkStyleHandle = osMessageQueueNew (1, sizeof(LED_BlinkProfile), &LED_BlinkStyle_attributes);

  /* creation of tagSignature */
  tagSignatureHandle = osMessageQueueNew (1, sizeof(LTTO_IR_SIGNATURE_t), &tagSignature_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of emitTag */
  emitTagHandle = osThreadNew(StartEmitTag, NULL, &emitTag_attributes);

  /* creation of LED */
  LEDHandle = osThreadNew(StartLEDFlash, NULL, &LED_attributes);

  /* creation of buttonMonitor */
  buttonMonitorHandle = osThreadNew(StartButtonMonitor, NULL, &buttonMonitor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1895;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 948;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 40-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 200-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, IR_LED1_Pin|IR_LED8_Pin|IR_LED7_Pin|IR_LED6_Pin
                          |IR_LED5_Pin|HeartbeatLED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, greenLED_Pin|PowerCtrl_Pin|IR_LED4_Pin|IR_LED3_Pin
                          |IR_LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : IR_LED1_Pin IR_LED8_Pin IR_LED7_Pin IR_LED6_Pin
                           IR_LED5_Pin HeartbeatLED_Pin */
  GPIO_InitStruct.Pin = IR_LED1_Pin|IR_LED8_Pin|IR_LED7_Pin|IR_LED6_Pin
                          |IR_LED5_Pin|HeartbeatLED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_L_Pin */
  GPIO_InitStruct.Pin = Button_L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Button_L_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : greenLED_Pin PowerCtrl_Pin IR_LED4_Pin IR_LED3_Pin
                           IR_LED2_Pin */
  GPIO_InitStruct.Pin = greenLED_Pin|PowerCtrl_Pin|IR_LED4_Pin|IR_LED3_Pin
                          |IR_LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RX_Pin */
  GPIO_InitStruct.Pin = RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RX_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	HAL_GPIO_WritePin(PowerCtrl_GPIO_Port, PowerCtrl_Pin, GPIO_PIN_SET);
	LED_BlinkProfile blinkStyle;
	/* Infinite loop */
	for(;;)
	{
		switch(presentState){
		case PROGRAM:
			if(LTTO_IR_RX_AC_NewSingleSignatureReady()) {
				LTTO_IR_RX_AC_GetSingleSignature(&tempSignature);
				if(tempSignature.signatureType == LTTO_IR_SIGNATURE_TYPE_TAG) {
					presentState = COUNTDOWN;
				}
				LTTO_IR_RX_AC_Clear();
			}
			osDelay(10);
			break;
		case COUNTDOWN:
			blinkStyle = SLOW_BLINK;
			xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
			osDelay(5000);
			if(presentState == SHUTDOWN){ //Lazy way to make sure we respond to button hold
				break;
			}

			blinkStyle = MEDIUM_BLINK;
			xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
			osDelay(3000);
			if(presentState == SHUTDOWN){ //Lazy way to make sure we respond to button hold
				break;
			}

			blinkStyle = FAST_BLINK;
			xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
			osDelay(2000);

			xQueueSendToBack(tagSignatureHandle, &tempSignature, 0);
			blinkStyle = SOLID_LED;
			xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
			presentState = DETONATE;
			break;
		case DETONATE:

			osDelay(5); //Wait for detonate to complete (or button to be held)
			break;

		case SHUTDOWN:
			blinkStyle = OCCASIONAL_BLIP;
			xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
			HAL_GPIO_WritePin(PowerCtrl_GPIO_Port, PowerCtrl_Pin, GPIO_PIN_RESET);
			break;
		}
		osDelay(10);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartEmitTag */
/**
 * @brief Function implementing the emitTag thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartEmitTag */
void StartEmitTag(void *argument)
{
  /* USER CODE BEGIN StartEmitTag */
	/* Infinite loop */
	for(;;)
	{
		//Our TX buffer
		int LTTO_IR_TX_txPulseMS[21];
		int LTTO_IR_TX_txPulseCount = 0;
		LTTO_IR_SIGNATURE_t signatureToSend;

		GPIO_TypeDef* portArray[8] = {IR_LED1_GPIO_Port, IR_LED2_GPIO_Port, IR_LED3_GPIO_Port,
				IR_LED4_GPIO_Port, IR_LED5_GPIO_Port, IR_LED6_GPIO_Port, IR_LED7_GPIO_Port, IR_LED8_GPIO_Port};
		uint16_t pinArray [8] = {IR_LED1_Pin, IR_LED2_Pin, IR_LED3_Pin, IR_LED4_Pin, IR_LED5_Pin,
				IR_LED6_Pin, IR_LED7_Pin, IR_LED8_Pin };

		xQueueReceive(tagSignatureHandle, &signatureToSend, portMAX_DELAY);
		LTTO_IR_CONV_SignatureToPulses(&signatureToSend, &LTTO_IR_TX_txPulseMS, &LTTO_IR_TX_txPulseCount);
		//LTTO_IR_SIGNATURE_t *sourceSignature, int destinationPulseTickMSBuffer[], int *destinationPulseCount
		for(int detonationRound = 0; detonationRound < 3; detonationRound++){ //Fire off 3 sets of 8 shots
			for(int emitter = 0; emitter < 8; emitter++) {
				for(int pulseIndex = 0; pulseIndex < LTTO_IR_TX_txPulseCount; pulseIndex++){
					if(pulseIndex % 2 == 0) {
						//Odd index, active IR period
						HAL_GPIO_WritePin(portArray[emitter], pinArray[emitter], GPIO_PIN_SET);
						osDelay(LTTO_IR_TX_txPulseMS[pulseIndex]);
					}
					else{
						//Even index, inactive IR period
						HAL_GPIO_WritePin(portArray[emitter], pinArray[emitter], GPIO_PIN_RESET);
						osDelay(LTTO_IR_TX_txPulseMS[pulseIndex]);
					}
				}
				osDelay(100); //100 ms inter-tag spacing
			}
		}
		presentState = SHUTDOWN; //kill the power
		osDelay(1000); //Sleep so other tasks can grab the Mutex
	}
  /* USER CODE END StartEmitTag */
}

/* USER CODE BEGIN Header_StartLEDFlash */
/**
 * @brief Function implementing the LED thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLEDFlash */
void StartLEDFlash(void *argument)
{
  /* USER CODE BEGIN StartLEDFlash */
	LED_BlinkProfile blinkStyle = ULTRA;
	uint16_t onTime;
	uint16_t offTime;
	uint32_t prevTime;
	bool LEDisOn;

	xQueueSendToBack(LED_BlinkStyleHandle, &blinkStyle, 0);
	/* Infinite loop */
	for(;;)
	{
		if(xQueueReceive(LED_BlinkStyleHandle, &blinkStyle, 0)){
			switch(blinkStyle){
			case ULTRA:
				onTime = 20;
				offTime = 20;
				break;
			case SLOW_BLINK:
				onTime = 500;
				offTime = 500;
				break;
			case MEDIUM_BLINK:
				onTime = 200;
				offTime = 200;
				break;
			case FAST_BLINK:
				onTime = 100;
				offTime = 100;
				break;
			case SOLID_LED:
				onTime = 65000;
				offTime = 0;
				break;
			case OCCASIONAL_BLIP:
				onTime = 20;
				offTime = 1000;
			}
			prevTime = HAL_GetTick();
			//Start with LED on
			LEDisOn = true;
			HAL_GPIO_WritePin(greenLED_GPIO_Port, greenLED_Pin, GPIO_PIN_RESET); //Turn on the LED (inverted)
			HAL_GPIO_WritePin(HeartbeatLED_GPIO_Port, HeartbeatLED_Pin, GPIO_PIN_SET); //Turn on the heartbeat LED
		}
		if(LEDisOn){
			if(HAL_GetTick()-prevTime > onTime){ //If on-time has elapsed
				HAL_GPIO_WritePin(greenLED_GPIO_Port, greenLED_Pin, GPIO_PIN_SET); //Turn off the LED (inverted)
				HAL_GPIO_WritePin(HeartbeatLED_GPIO_Port, HeartbeatLED_Pin, GPIO_PIN_RESET); //Turn off the heartbeat LED
				LEDisOn = false;
				prevTime = HAL_GetTick(); //Reset timer so we can time the off time
			}
		}
		else{
			if(HAL_GetTick()-prevTime > offTime){ //If off-time has elapsed
				HAL_GPIO_WritePin(greenLED_GPIO_Port, greenLED_Pin, GPIO_PIN_RESET); //Turn on the LED (inverted)
				HAL_GPIO_WritePin(HeartbeatLED_GPIO_Port, HeartbeatLED_Pin, GPIO_PIN_SET); //Turn on the heartbeat LED
				LEDisOn = true;
				prevTime = HAL_GetTick(); //Reset timer so we can time the off time
			}
		}
		osDelay(10);
	}

  /* USER CODE END StartLEDFlash */
}

/* USER CODE BEGIN Header_StartButtonMonitor */
/**
 * @brief Function implementing the buttonMonitor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartButtonMonitor */
void StartButtonMonitor(void *argument)
{
  /* USER CODE BEGIN StartButtonMonitor */
	int resetCount = 30; //for button push
	int failToProgramCount = 60 * 10; //Shutdown if not programmed within 60s
	int interval = 100; //ms
	int currentCount = resetCount;
	/* Infinite loop */
	for(;;)
	{
		if(HAL_GPIO_ReadPin(Button_L_GPIO_Port, Button_L_Pin)==GPIO_PIN_RESET){ //button pressed
			currentCount--; //decrement the countdown
		}
		else{
			currentCount = resetCount; //reset the countdown
		}
		if(presentState == PROGRAM){
			failToProgramCount--;
		}
		if(currentCount <= 0 || failToProgramCount <= 0){
			presentState = SHUTDOWN; //kill the power
		}
		osDelay(interval);
	}
  /* USER CODE END StartButtonMonitor */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM17 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

	static uint16_t IR_RX_SensorTicks = 0;
	static bool IR_RX_LastState;
	static bool IR_RX_currentState;
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
	if (htim == &htim6 )  { // 0.2ms Interrupt
		if(HAL_GPIO_ReadPin(RX_GPIO_Port, RX_Pin) == GPIO_PIN_SET){
			IR_RX_currentState = true;
		}
		else{
			IR_RX_currentState = false;
		}
		//Handle IR reception
		//If the state is the same as last time
		if(IR_RX_currentState == IR_RX_LastState) {
			//If this pulse has gone longer than 10ms, and we haven't already decided we've timed out...
			if((IR_RX_SensorTicks / LTTO_IR_RX_TICKS_PER_MS) > 10 && IR_RX_SensorTicks != IR_RX_SENSOR_TICKS_GAVE_UP) {
				//Pass it into the RX processor
				//Invert the sensor state, since IR receivers are active-low
				LTTO_IR_RX_AC_NewIRPulseReceived(IR_RX_SensorTicks, !IR_RX_LastState);
				//And mark this pulse as having already been processed
				IR_RX_SensorTicks = IR_RX_SENSOR_TICKS_GAVE_UP;
			} else {
				//Otherwise, increment the counter
				IR_RX_SensorTicks++;
			}
		} else {
			//If we haven't already processed this pulse due to timing out...
			if(IR_RX_SensorTicks != IR_RX_SENSOR_TICKS_GAVE_UP) {
				//Pass it into the processor.
				//Invert the state, since IR receivers are active-low.
				LTTO_IR_RX_AC_NewIRPulseReceived(IR_RX_SensorTicks, !IR_RX_LastState);
			}
			//And reset the timer
			IR_RX_SensorTicks = 0;
			//And store the current state of the pin
			IR_RX_LastState = IR_RX_currentState;
		}
	}

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
