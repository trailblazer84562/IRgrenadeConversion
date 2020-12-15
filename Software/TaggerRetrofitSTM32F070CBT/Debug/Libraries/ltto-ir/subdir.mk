################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/ltto-ir/LTTO_IR_Conversions.c \
../Libraries/ltto-ir/LTTO_IR_RX.c \
../Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.c \
../Libraries/ltto-ir/LTTO_IR_TX.c 

OBJS += \
./Libraries/ltto-ir/LTTO_IR_Conversions.o \
./Libraries/ltto-ir/LTTO_IR_RX.o \
./Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.o \
./Libraries/ltto-ir/LTTO_IR_TX.o 

C_DEPS += \
./Libraries/ltto-ir/LTTO_IR_Conversions.d \
./Libraries/ltto-ir/LTTO_IR_RX.d \
./Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.d \
./Libraries/ltto-ir/LTTO_IR_TX.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/ltto-ir/LTTO_IR_Conversions.o: ../Libraries/ltto-ir/LTTO_IR_Conversions.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F070xB -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Libraries/ltto-ir -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Libraries/ltto-ir/LTTO_IR_Conversions.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Libraries/ltto-ir/LTTO_IR_RX.o: ../Libraries/ltto-ir/LTTO_IR_RX.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F070xB -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Libraries/ltto-ir -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Libraries/ltto-ir/LTTO_IR_RX.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.o: ../Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F070xB -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Libraries/ltto-ir -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Libraries/ltto-ir/LTTO_IR_RX_AutoConvert.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Libraries/ltto-ir/LTTO_IR_TX.o: ../Libraries/ltto-ir/LTTO_IR_TX.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F070xB -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Libraries/ltto-ir -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Libraries/ltto-ir/LTTO_IR_TX.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

