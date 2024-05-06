#include "Emisor.h"


/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_emisor;                        // thread id
 
void Thread_emisor (void *argument);                   // thread function
static GPIO_InitTypeDef GPIO_InitStruct;
 
static uint32_t frecuencias[10] = {
  10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000
};
 TIM_HandleTypeDef htim1; 
static TIM_OC_InitTypeDef pwm_output;
 
int Init_emisor (void) {
 
  tid_emisor = osThreadNew(Thread_emisor, NULL, NULL);
  if (tid_emisor == NULL) {
    return(-1);
  }
  Init_output_pin();
  return(0);
}
 
void Thread_emisor (void *argument) {
 uint8_t key[4] = {1, 2, 3, 4}; // Ejemplo de clave
 
  while (1) {
   osThreadFlagsWait(0x01,osFlagsWaitAny,50);
      for (int i = 0; i < 4; i++) {
      // Generar el pulso PWM para el dígito actual
      generate_pwm(frecuencias[key[i]],1000);
//generate_pwm(2000,100);
      // Esperar un breve periodo entre dígitos
      osDelay(50);
    }

    // Esperar un periodo más largo entre claves
//    osDelay(1000);
  }
  
}

void Init_output_pin(void){

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
  htim1.Init.Period =  1000 -1;  	
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

void generate_pwm(uint32_t frequency, uint32_t duration) {
  // Calcular el valor del duty cycle
  uint32_t duty_cycle = (1000000 / frequency) / 2;
  
  htim1.Init.Period = frequency-1; 
//pwm_output.Pulse= (htim1.Init.Period/2)-1;

//	

  // Configurar el timer
  TIM_OC_InitTypeDef sConfigOC;
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = duty_cycle;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  
  HAL_TIM_PWM_Init(&htim1);  
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  // Iniciar el timer
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  // Esperar la duración del pulso
  osDelay(duration);

  // Detener el timer
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

/*
// Definición de los pines de salida PWM
#define PWM_PIN1 GPIO_PIN_10
#define PWM_PIN2 GPIO_PIN_11
#define PWM_PIN3 GPIO_PIN_12
#define PWM_PIN4 GPIO_PIN_13
#define PWM_PORT GPIOB

// Tabla de frecuencias para cada dígito
uint32_t frequencies[10] = {
  1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800
};

// Variables para la generación de PWM
TIM_HandleTypeDef htim1;

// Función para generar un pulso PWM de una frecuencia específica
void generate_pwm(uint32_t frequency, uint32_t duration) {
  // Calcular el valor del duty cycle
  uint32_t duty_cycle = (1000000 / frequency) / 2;

  // Configurar el timer
  TIM_OC_InitTypeDef sConfigOC;
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = duty_cycle;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

  // Iniciar el timer
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  // Esperar la duración del pulso
  osDelay(duration);

  // Detener el timer
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

// Tarea para generar la clave numérica
void task_generate_key(void *argument) {
  uint8_t key[4] = {1, 2, 3, 4}; // Ejemplo de clave

  while (1) {
    for (int i = 0; i < 4; i++) {
      // Generar el pulso PWM para el dígito actual
      generate_pwm(frequencies[key[i]], 100);

      // Esperar un breve periodo entre dígitos
      osDelay(50);
    }

    // Esperar un periodo más largo entre claves
    osDelay(1000);
  }
}

int main(void) {
  // Inicializar el hardware
  HAL_Init();

  // Configurar los pines de salida PWM
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = PWM_PIN1 | PWM_PIN2 | PWM_PIN3 | PWM_PIN4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);

  // Configurar el timer PWM
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000 - 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim1);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIM_MasterConfig(&htim1, &sMasterConfig);
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 5000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_
*/


