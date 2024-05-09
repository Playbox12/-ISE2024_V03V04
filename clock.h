#ifndef __CLOCK_H
#define __CLOCK_H
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stdio.h"
#include "time.h"
#include "cmsis_os2.h"
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "lcd.h"

int Init_Thclock(void);
static void time_cback (uint32_t seconds, uint32_t seconds_fraction);
bool get_time (void);


extern RTC_HandleTypeDef hrtc;


void RTC_Initialize(void);
void Escribir_hora_rtc(void);
void calendar_config(void);



#endif
