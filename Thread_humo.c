#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread_humo.h"
#include "stm32f4xx_hal.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_humo;                        // thread id
uint16_t  status; 


ADC_HandleTypeDef adchandle;
void Thread_humo (void *argument);                   // thread function

 
int Init_Thread_humo (void) {
 
  tid_Thread_humo = osThreadNew(Thread_humo, NULL, NULL);
  if (tid_Thread_humo == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_humo (void *argument) {

	LED_Init();
	HUMO_init();

  while (1) {

		
		status = osThreadFlagsWait(0x03U, osFlagsWaitAny, 100);
		if(status == 0x03){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
		}
		osThreadYield();                            // suspend thread
  }
}


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
		osThreadFlagsSet(tid_Thread_humo,0x03);
		}
      
}
 void LED_Init(void) { //configuracion de los leds pin0=verde pin14=rojo
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
}

