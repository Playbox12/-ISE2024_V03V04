#ifndef THREAD_TECLADO_H
#define THREAD_TECLADO_H
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#define MAX_LONGITUD_CONTRASENIA 4

extern osThreadId_t tid_Teclado;                        // thread id
char teclado_read(void); 
void multiplex (void);
void guardar_pssw(char tecla);
extern char pssw_T[MAX_LONGITUD_CONTRASENIA+1]; // +1 para el terminador nulo ('\0')

extern uint8_t FILA,COLUMNA;
extern char tecla;
extern int longi;
#endif

