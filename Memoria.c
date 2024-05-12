#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h" 
#include <stdio.h>
#include "at24cxx.h"
#include "i2c.h"
#include "iwdg.h"

uint8_t wData[50];
uint8_t rData[50];
uint8_t n_plaza = 0;
int datos[5]={1,2,3,4,5};

int Init_Thread (void); 
osThreadId_t tid_Thread_Memory;                        // thread id
 
void Thread_Memory (void *argument);                   // thread function
 
int Init_Thread_Memory (void) {
 
  tid_Thread_Memory = osThreadNew(Thread_Memory, NULL, NULL);
  if (tid_Thread_Memory == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_Memory (void *argument) {
	uint8_t memo= 0x00;
	
	
  while (1) {
    if(at24_isConnected()){
    for(int i = 0; i < 5; i++){
			wData[i] = datos[i];
		}
		osDelay(2000);
		at24_write(memo, wData, 0x05, 100);
		osDelay(1000);
		
		at24_read(memo, rData, 0x05, 100);
		osDelay(1000);
		}
	}
}
