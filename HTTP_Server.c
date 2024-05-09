/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "lcd.h"

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);

extern char lcd_text[2][20+1];
extern char rtc_text[2][20+1];
extern char parking_text[2][20+1];
extern char alarma_text[1][20+1];

extern osThreadId_t TID_Display;
extern osThreadId_t TID_RTC;
extern osThreadId_t TID_Reset;
extern osThreadId_t TID_Contrasenia;
extern osThreadId_t TID_Alarma;

extern osTimerId_t tim_id1; 

RTC_HandleTypeDef hrtc;

GPIO_InitTypeDef GPIO_InitStruct;

char pssw_S[5];
char parking_text[2][20+1] = { "PSSW  1",
                               "PSSW line 2" };

char lcd_text[2][20+1] = { "LCD  1",
                           "LCD line 2" };

char rtc_text[2][20+1] = { "RTC line 1",
                           "RTC line 2" };

char alarma_text[1][20+1] = { "Desactivada" };

char alarma[80];

int32_t val_aux = 0;
                           
//void LED_Init(void);
/* Thread IDs */
osThreadId_t TID_Display;   
osThreadId_t TID_RTC;
osThreadId_t TID_Reset;	
osThreadId_t TID_Contrasenia;
osThreadId_t TID_Alarma;
/* Thread declarations */

static void Display  (void *arg);
static void RTC_xd (void *arg);
static void Contrasenia (void *arg);
static void Alarma (void *arg);
__NO_RETURN void app_main (void *arg);

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
    osThreadFlagsSet (TID_Display, 0x01);
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {
	
  static char    buf[24];
  (void)arg;

  while(1) {
    /* Wait for signal from DHCP */
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
		
		//clean();
		writeline1(lcd_text[0]);
		writeline2(lcd_text[1]);
		LCD_update();
		

  }
}

/*----------------------------------------------------------------------------
  Thread 'RTC': RTC valores handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void RTC_xd (void *arg) {
	int horas, minutos, segundos, dias, meses, anios, status;
	RTC_DateTypeDef sdate;
  RTC_TimeTypeDef stime;
	
	(void)arg;

  while(1) {
    /* Wait for signal from DHCP */
    status=osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
			
		sscanf(&rtc_text[0][0], "Hora: %02d:%02d:%02d", &horas,&minutos,&segundos);
    sscanf(&rtc_text[1][6], "%02d-%02d-%04d", &dias,&meses,&anios);
		
		stime.Hours=horas;
		stime.Minutes=minutos;
		stime.Seconds=segundos;
		sdate.Date=dias;
		sdate.Month=meses;
		sdate.Year=anios-2000;
			
		HAL_RTC_SetTime(&hrtc,&stime,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&sdate,RTC_FORMAT_BIN);
  }
}


/*----------------------------------------------------------------------------
  Thread 'RTC': RTC reset handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void RTC_reset (void *arg) {
	RTC_DateTypeDef sdate;
  RTC_TimeTypeDef stime;
	bool res=false;
	
	(void)arg;

  while(1) {
    /* Wait for signal from DHCP */
    osThreadFlagsWait (0x02U, osFlagsWaitAny, osWaitForever);
		
		stime.Hours=11;
		stime.Minutes=11;
		stime.Seconds=11;
		sdate.Date=11;
		sdate.Month=11;
		sdate.Year=11;
		
		HAL_RTC_SetTime(&hrtc,&stime,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&sdate,RTC_FORMAT_BIN);
  }
}

/*----------------------------------------------------------------------------
  Thread 'Contrasenia': Servidor valores handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Contrasenia (void *arg) {
	int contrasenia,status;

	
	(void)arg;

  while(1) {
    /* Wait for signal from DHCP */
    status=osThreadFlagsWait (0x04U, osFlagsWaitAny, osWaitForever);
			
		sscanf(parking_text[0], "%d", &contrasenia);  //Recojo el valor del servidor
    sprintf(pssw_S, "%d",contrasenia);            //Lo guardo como char para luego poder compararlo
		
  }
}


/*----------------------------------------------------------------------------
  Thread 'Alarma': Servidor valores handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Alarma (void *arg) {
	int status;

	(void)arg;

  while(1) {
    /* Wait for signal from DHCP */
    status=osThreadFlagsWait (0x03, osFlagsWaitAny, osWaitForever);
    sprintf(alarma_text[0],"PELIGRO");
  }
}



/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

  netInitialize ();

  TID_Display = osThreadNew (Display,  NULL, NULL);
  TID_RTC = osThreadNew (RTC_xd,  NULL, NULL);
	TID_Reset = osThreadNew (RTC_reset,  NULL, NULL);
  TID_Contrasenia = osThreadNew (Contrasenia,  NULL, NULL);
  TID_Alarma = osThreadNew (Alarma,  NULL, NULL);
  osThreadExit();
}
