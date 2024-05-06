#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Receptor.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_receptor;                        // thread id
static GPIO_InitTypeDef GPIO_InitStruct;

void Thread_receptor (void *argument);                   // thread function
// Variables para el cálculo de la frecuencia
 uint32_t last_rising_edge = 0;
 uint32_t current_time = 0;
 uint32_t freq = 0;
 uint32_t periodo=0;
static uint32_t frecuencias[10] = {
  10, 12, 14, 16, 18, 20, 22, 24, 26, 28
};

int Init_receptor (void) {
 
  tid_receptor = osThreadNew(Thread_receptor, NULL, NULL);
  if (tid_receptor == NULL) {
    return(-1);
  }
 Init_input_pin();
  return(0);
}
 
void Thread_receptor (void *argument) {
 uint8_t rec_key[4];
 uint8_t key[4] = {1, 2, 3, 4}; // Ejemplo de clave
 uint8_t i =0;
 uint8_t j =0;
  while (1) {


    // Leer la frecuencia
   osThreadFlagsWait(0x01,osFlagsWaitAny,osWaitForever);
    for(i=0;i<10;i++){
    if(periodo==frecuencias[i])
     rec_key[j] = i;
    j++; if(j==4) j=0;
  }
    osDelay(150);
 
    for(i=0;i<4;i++){
    if(key[i]==rec_key[i]){
    ////// enviar contraseña correcta
    }
  }
  }
}

// Función para detectar el flanco ascendente de la señal PWM
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  //if (GPIO_Pin == GPIO_PIN_0/*PWM_PIN*/) {
    // Obtener el tiempo actual
    current_time = HAL_GetTick();

    // Calcular el período entre flancos ascendentes
     periodo = current_time - last_rising_edge;

    // Calcular la frecuencia
 //   freq = 1000000 / periodo;

    // Actualizar el tiempo del último flanco ascendente
    last_rising_edge = current_time;
 // }
}

void Init_input_pin(void){
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Habilitar la interrupción para el pin de entrada PWM
  //HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/*
// Definición del pin de entrada PWM
#define PWM_PIN GPIO_PIN_10
#define PWM_PORT GPIOB

// Variables para el cálculo de la frecuencia
volatile uint32_t last_rising_edge = 0;
volatile uint32_t current_time = 0;
volatile uint32_t frequency = 0;

// Función para detectar el flanco ascendente de la señal PWM
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == PWM_PIN) {
    // Obtener el tiempo actual
    current_time = HAL_GetTick();

    // Calcular el período entre flancos ascendentes
    uint32_t period = current_time - last_rising_edge;

    // Calcular la frecuencia
    frequency = 1000000 / period;

    // Actualizar el tiempo del último flanco ascendente
    last_rising_edge = current_time;
  }
}

// Tarea para leer la frecuencia y mostrarla por el puerto serie
void task_read_frequency(void *argument) {
  while (1) {
    // Esperar 1 segundo
    osDelay(1000);

    // Leer la frecuencia
    uint32_t current_frequency = frequency;

    // Mostrar la frecuencia por el puerto serie
    printf("Frecuencia: %d Hz\n", current_frequency);
  }
}

int main(void) {
  // Inicializar el hardware
  HAL_Init();

  // Configurar el pin de entrada PWM
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = PWM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);

  // Habilitar la interrupción para el pin de entrada PWM
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  // Crear la tarea para leer la frecuencia
  osThreadDef(task_read_frequency, task_read_frequency, 1, 0);
  osThreadCreate(osThread(task_read_frequency), NULL);

  // Iniciar el scheduler
  osKernelStart();

  // Bucle infinito
  while (1) {
    // No se hace nada aquí
  }
}
*/




