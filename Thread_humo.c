#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread_humo.h"
#include "stm32f4xx_hal.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_humo;                        // thread id
uint16_t  status; 
float value_adc;
// float scope;
// float coord;
//float concentracion; 
//float rs;
//float Ro = 10;
//float humo;
//	float punto0[] = {2.3,0.544};
//  float punto1[] = {4,-0.2218};

	int raw_adc;
//	int raw;
ADC_HandleTypeDef adchandle;
void Thread_humo (void *argument);                   // thread function
//float Humo_curva[3]= {2.3, 0.53, -0.43};
 
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
 //ADC1 configuration
//  scope = (punto1[1] - punto0[1]) / (punto1[0] - punto0[0]); //-0.45
//  coord = punto0[1] - (punto0[0] * scope); //1.579
			ADC1_pins_F429ZI_config(); //specific PINS configuration	
			ADC_Init_Single_Conversion(&adchandle , ADC1);
	//	Ro = SensorCalibration();
  while (1) {

		// Insert thread code here...
	 //raw_adc = ADC_getraw_adc(&adchandle , 10);
		//value_adc = raw_adc *(5.0/4096.0);
		
		//humo = calculo_concentracion();
		
//		status = osThreadFlagsWait(0x03U, osFlagsWaitAny, 100);
//		if(status == 0x03){
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
//		}
		osThreadYield();                            // suspend thread
  }
}

//float SensorCalibration(void){
//int i; 
//float val = 0;
//	for(i=0;i<50;i++){
//		val += getMQResistence(ADC_getraw_adc(&adchandle , 10));
//		osDelay(600);
//	}
//	val = val/50;
//	val = val/10;
//	
//	return val;
//}
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

//float calculo_concentracion(void){
//		//concentracion = getConcentracion(rs_med/10);
//	
//	return (pow(10,(((log10(readMQ()/Ro)-Humo_curva[1])/Humo_curva[2]) + Humo_curva[0])));;
//}

//float readMQ(void){
//	
//	int i;
////int raw;
//	for(i=0; i<5; i++){//read sample times
//		//raw = ADC_getraw_adc(&adchandle , 10);
//		rs+= getMQResistence(ADC_getraw_adc(&adchandle , 10));
//		osDelay(50);//READ_SAMPLE_INTERVAL
//	}
//	return rs/5;
//}

//float getMQResistence(int raw_adc){
//	return ((50.0*(4096-raw_adc)/raw_adc)); 
//}
//float getConcentracion(float rs_ro_ratio){
//	return pow(10, coord + scope * log(rs_ro_ratio));
//}
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){ 
  
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)== 0){
		osThreadFlagsSet(tid_Thread_humo,0x03);
		}
      
}
// void LED_Init(void) { //configuracion de los leds pin0=verde pin14=rojo
//  GPIO_InitTypeDef GPIO_InitStruct;
//  __HAL_RCC_GPIOB_CLK_ENABLE();

//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  
//  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
//  HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
//}

