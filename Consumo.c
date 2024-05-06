#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#define RESOLUTION_12B 4096U
#define VREF 5.0f
#define BATTERYCAP 800.0f


float tiempototal;
float porcentaje;
int segundos = 0;
/**
  * @brief config the use of analog inputs ADC123_IN10 and ADC123_IN13 and enable ADC1 clock
  * @param None
  * @retval None
  */
void ADC1_pins_F429ZI_config(){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/*PC0     ------> ADC1_IN10
    PC3     ------> ADC1_IN13
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
  }
/**
  * @brief Initialize the ADC to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
int ADC_Init_Single_Conversion(ADC_HandleTypeDef *hadc, ADC_TypeDef  *ADC_Instance)
{
	
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc->Instance = ADC_Instance;
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.ScanConvMode = DISABLE;
  hadc->Init.ContinuousConvMode = DISABLE;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.NbrOfConversion = 1;
  hadc->Init.DMAContinuousRequests = DISABLE;
 hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    return -1;
  }
 
	
	return 0;

}

/**
  * @brief Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param channel number
	* @retval voltage in float (resolution 12 bits and VRFE 5.0
  */
float ADC_getCurrent(ADC_HandleTypeDef *hadc, uint32_t Channel)
	{
		ADC_ChannelConfTypeDef sConfig = {0};
		HAL_StatusTypeDef status;

		uint32_t raw = 0;
		float current = 0;
		float shuntResistance = 0.1;
		
		 /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = Channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    return -1;
  }
		
		HAL_ADC_Start(hadc);
		
		do {
			status = HAL_ADC_PollForConversion(hadc, 0); //This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
		}while(status != HAL_OK);
		
		raw = HAL_ADC_GetValue(hadc);
		
		float voltageAcrossShunt = raw * VREF / RESOLUTION_12B;
		current = voltageAcrossShunt / shuntResistance;
		
		float batterytiempo = BATTERYCAP/current;
		tiempototal = batterytiempo * 3600;
		
		return current;

}
	
/*----- Periodic Timer Example -----*/
osTimerId_t tim_id2;                            
static uint32_t exec2;                          
 
// Periodic Timer Function
static void Timer2_Callback (void const *arg) {
	
	if( segundos < tiempototal){
		segundos++;
	}else if(segundos>=tiempototal)
		segundos=0;
	
	porcentaje = (100 - (segundos*25/5100));
  
}
 

int Init_Timers (void) {
  osStatus_t status;                            
 
  // Create periodic timer
  exec2 = 2U;
  tim_id2 = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_id2 != NULL) {  
    
    status = osTimerStart(tim_id2, 1000U);            
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}
	

