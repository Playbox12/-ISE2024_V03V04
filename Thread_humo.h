#include "cmsis_os2.h"
#include "adc.h"
#include <math.h>
#include <stdio.h>
#ifndef __HUMO_H



	

//INICIAR THREAD HUMO
int Init_Thread_humo (void);
void HUMO_init(void);
void LED_Init(void);
float readMQ(void);
float getMQResistence(int raw_adc);
float getConcentracion(float rs_ro_ratio);
float calculo_concentracion(void);
float SensorCalibration(void);
#endif
