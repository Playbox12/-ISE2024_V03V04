#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "humo.h"
#include "stm32f4xx_hal.h"
void HUMO_init(void){
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin=GPIO_PIN_3;
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING; 
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed= GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}


 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){ 
  
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)== 0){
		osThreadFlagsSet(tid_PWM,0x03);
		}
      
}