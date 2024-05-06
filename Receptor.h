#ifndef RECEPTOR_H
#define RECEPTOR_H

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file



//extern GPIO_InitTypeDef GPIO_InitStruct;

extern osThreadId_t tid_receptor;
int Init_receptor(void);

extern uint32_t last_rising_edge;
extern uint32_t current_time;
extern uint32_t freq;
extern uint32_t periodo;
void Init_input_pin(void);

#endif

