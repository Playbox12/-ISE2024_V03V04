#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "pwm.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
 //COLA
osMessageQueueId_t mid_MsgQueuePWM;
MSGQUEUE_OBJ_t CT;
int Init_MsgQueue_PWM (void);
uint16_t  status;

//THREAD DEL PWM 
osThreadId_t tid_PWM;                        // thread id
void PWM (void *argument);                   // thread function
 
//THREAD DEL TEST DEL PWM
void Th_PWM_Test(void *argument); 
osThreadId_t tid_Th_PWM_Test; 


//TIMER
TIM_HandleTypeDef htim1; 
TIM_OC_InitTypeDef pwm_output;

int ciclo_T = 0;

void PWM_init(void);

int Init_PWM (void) {
 
  tid_PWM = osThreadNew(PWM, NULL, NULL);
  if (tid_PWM == NULL) {
    return(-1);
  }
	//PWM_init();
  Init_MsgQueue_PWM();
  Init_PWM_Test();
  return(0);
}
 
void PWM (void *argument) {
 MSGQUEUE_OBJ_t msg;
	PWM_init();
  while (1) {
     // Insert thread code here...
	  status = osThreadFlagsWait(0x03U, osFlagsWaitAny, 0);
		if(status == 0x01){
			PWM_activar();
			PWM_cambio(2270);
			HAL_Delay(200);
			PWM_desactivar();	
			HAL_Delay(200);
			PWM_activar();
			PWM_cambio(2200);
			HAL_Delay(300);
			PWM_desactivar();	
			HAL_Delay(30);
			PWM_activar();
			PWM_cambio(2140);
			HAL_Delay(300);
			PWM_desactivar();
		}
		else if(status == 0x02){
			PWM_activar();
			HAL_Delay(200);
			PWM_desactivar();	
			HAL_Delay(300);
			PWM_activar();
			PWM_cambio(7000);
			HAL_Delay(700);
			PWM_desactivar();
		}
		else if(status == 0x03){
		PWM_activar();
		HAL_Delay(500);
		PWM_cambio(FREQ_TONO2);
		HAL_Delay(1000);
		PWM_cambio(FREQ_TONO1);
		
		}else{PWM_desactivar();}
		
//		int i;
//		for(i=0; i<5; i++){
//		PWM_activar();
//		HAL_Delay(500);
//		PWM_cambio(FREQ_TONO2);
//		HAL_Delay(1000);
//		PWM_cambio(FREQ_TONO1);
//		}
//		PWM_desactivar();
		osThreadYield();                            // suspend thread
  }
}

//Gets the queue ID
//osMessageQueueId_t getModPWMQueueID(void){
//	return mid_MsgQueuePWM;
//}

void PWM_init(void){
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitStruct.Pin=GPIO_PIN_9;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP; 
	GPIO_InitStruct.Pull=GPIO_PULLDOWN;
	GPIO_InitStruct.Alternate=GPIO_AF1_TIM1; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
  //Timer 
  __HAL_RCC_TIM1_CLK_ENABLE(); 
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 167 ; 
  htim1.Init.Period =  FREQ_TONO1 -1;  	
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htim1);

  HAL_TIM_PWM_Init(&htim1);
  
  pwm_output.OCMode=TIM_OCMODE_PWM1;
  pwm_output.OCPolarity = TIM_OCPOLARITY_HIGH;
  pwm_output.OCFastMode = TIM_OCFAST_DISABLE;
	pwm_output.Pulse= (htim1.Init.Period/2)-1;
	HAL_TIM_OC_Init(&htim1);

	 //poner pwm en vez de oc en estas tres
	HAL_TIM_PWM_ConfigChannel(&htim1,&pwm_output,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

void PWM_cambio(uint32_t frecuencia){
  htim1.Init.Period = frecuencia-1; 
	pwm_output.Pulse= (htim1.Init.Period/2)-1;
	//HAL_TIM_Base_Init(&htim1);
	HAL_TIM_PWM_Init(&htim1);
}

void PWM_desactivar(){
	HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
}

void PWM_activar(){
HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

void clave_correcta(){
osThreadFlagsSet(tid_PWM,0x01);
}

void clave_erronea(){
osThreadFlagsSet(tid_PWM,0x02);
}

void alarma(){
osThreadFlagsSet(tid_PWM,0x03);
}
int Init_MsgQueue_PWM (void) {
 
  mid_MsgQueuePWM = osMessageQueueNew(MSGQUEUE_OBJECTS , sizeof(MSGQUEUE_OBJ_t), NULL);
  if (mid_MsgQueuePWM == NULL) {
    return (-1);
  }
  return(0);
}
int Init_PWM_Test(void) {
 
  tid_Th_PWM_Test = osThreadNew(Th_PWM_Test, NULL, NULL);
  if (tid_Th_PWM_Test == NULL) {
    return(-1);
  }	
  return(0);
}
 
void Th_PWM_Test (void *argument) {


	
		alarma();	
		
    		
  
}
