#ifndef __LCD_H
#define __LCD_H

/*Includes*/
#include "Driver_SPI.h"

#include "stdio.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#define SIZE_MSGQUEUE_LCD			10

/*Funciones*/
void initTIM7(void);
void delay (uint32_t n_microsegundos);
void LCD_RESET(void);
void LCD_wr_data(unsigned char data);
void LCD_wr_cmd(unsigned char cmd);
void LCD_init(void);
void LCD_update(void);
void symbolToLocalBuffer_L1(uint8_t symbol);
void symbolToLocalBuffer_L2(uint8_t symbol);
void symbolToLocalBuffer(uint8_t line,uint8_t symbol);
void clean(void);
void Clean_L1(void);
void Clean_L2(void);
void writeline1 (char *letras);
void writeline2 (char *letras);

typedef struct {                               
  uint8_t linea;
  char bufferLcd[25];
} MSGQUEUE_LCD_t;



#endif 


