#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "humo.h"
#include "pwm.h"
#include "stm32f4xx_hal.h"
void HUMO_init(void){
	GPIO_InitTypeDef GPIO_InitStruct={0};
//	__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOC_CLK_ENABLE();
	//GPIO_InitStruct.Pin=GPIO_PIN_3;
  GPIO_InitStruct.Pin=GPIO_PIN_13;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING; 
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed= GPIO_SPEED_HIGH;
	//HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	//HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


