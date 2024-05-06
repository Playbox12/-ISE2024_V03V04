#include "cmsis_os2.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"

#define PLAZAS_MAX 30
#define DIST_MAX   2000 //mm
#define DIST_MIN   500  //mm
#define DIST_MED   1000 //mm

/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function
static void Timer1_Callback (void const *arg);


osThreadId_t tid_distancia;
int Init_Thread(void);
void Thread(void *argument);


TIM_HandleTypeDef htim_trig;

uint32_t distancia_cm = 0;
uint8_t cnt_plazas = 0;
//bool n_residentes; // Considera incluir esta variable como global
void control_aforo(int d);

int Init_Thread(void) {
  tid_distancia = osThreadNew(Thread, NULL, NULL);
  if (tid_distancia == NULL) {
    return (-1);
  }

  return (0);
}

void Thread(void *argument) {
	
  HAL_TIM_Base_Start(&htim_trig);
	
  while (1) {
    
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);  // Activar el pin TRIG
		
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Desactivar el pin TRIG

    // Esperar hasta que el pin ECHO se active
    while (!HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3)) {}

    // Iniciar temporizador
    __HAL_TIM_SET_COUNTER(&htim_trig, 0);

    // Esperar hasta que el pin ECHO se desactive
    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3)) {}

    // Calcular el tiempo transcurrido (en microsegundos)
    uint32_t tiempo = __HAL_TIM_GET_COUNTER(&htim_trig);

    // Calcular la distancia en cm
    distancia_cm = (tiempo * 0.0343) / 2; // La velocidad del sonido es de aproximadamente 343 m/s

    control_aforo(distancia_cm);
    osThreadYield(); // Suspender el hilo
  }
}

void control_aforo(int d) {
  // Si la distancia medida es menor o igual a DIST_MIN y hay residentes
  // Incrementa el contador de plazas ocupadas
  if (d <= DIST_MIN ){//&& n_residentes == true) {
    cnt_plazas++;
  }

  // Si la distancia medida está entre DIST_MIN y DIST_MED
  // (es decir, un coche está saliendo)
  // Decrementa el contador de plazas ocupadas
  if (d > DIST_MIN && d <= DIST_MED) {
    cnt_plazas--;
  }
}




