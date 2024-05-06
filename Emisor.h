#ifndef EMISOR_H
#define EMISOR_H

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file



//extern GPIO_InitTypeDef GPIO_InitStruct;

extern osThreadId_t tid_emisor;
int Init_emisor(void);
void generate_pwm(uint32_t frequency, uint32_t duration);
void Init_output_pin(void);


#endif

