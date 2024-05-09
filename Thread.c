#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stdio.h"
#include "lcd.h" 
#include "stm32f4xx_hal.h"
#include "string.h"
#define MAX_LONGITUD_CONTRASENIA 4
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread;                        // thread id
 
void Thread (void *argument);                   // thread function
uint8_t secuencia;
uint8_t FILA,COLUMNA;
extern osTimerId_t tim_50ms;                            // timer id
char tecla;
char teclado_read(void); 
void multiplex (void);
void guardar_pssw(char tecla);
char contrasenia[MAX_LONGITUD_CONTRASENIA + 1]; // +1 para el terminador nulo ('\0')
int longi = 0;
char pssw_servi[] = "1234";
char mensaje;

		
char teclas [4][4] = {{'1','2','3','A'},
											 {'4','5','6','B'},
											 {'7','8','9','C'},
											 {'*','0','#','D'}};
int i,j;
											 
int Init_Thread (void) {
 
  tid_Thread = osThreadNew(Thread, NULL, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {	
		//osTimerStart(tim_50ms,200U); //para los rebotes
    multiplex();
		secuencia=osThreadFlagsWait(0x00000001U,osFlagsWaitAny,50);
     if(secuencia==0x00000001U){
      	sprintf(pagina, "pulsado");
			  LCD_imprimir_L1(pagina);
	      LCD_Update();	
			  longi++;
			  tecla = teclado_read();		 	
	      LCD_imprimir_L2(&tecla);
	      LCD_Update();
			  guardar_pssw(tecla);
     }
		}
			
    osThreadYield();                            // suspend thread
 }

void guardar_pssw(char tecla){
	
	if(longi == 1)
		contrasenia[0] = tecla;
	if(longi == 2)
		contrasenia[1] = tecla;
	if(longi == 3)
		contrasenia[2] = tecla;
	if(longi == 4)
		contrasenia[3] = tecla;	
	
	if(longi == 4){	 
		 LCD_imprimir_L1(&contrasenia);
	   LCD_Update();
	}
 // Comparar la contraseña con otra cadena
  if ((longi == 4) & (strcmp(contrasenia, pssw_servi) == 0)){ // Cambia "1234" por la contraseña a comparar   
	 
		sprintf(pagina, "Contrasenia correcta");
	  LCD_imprimir_L1(pagina);
	  LCD_Update();
	}	
  if((longi == 4) & (strcmp(contrasenia, pssw_servi) != 0)){
		sprintf(pagina, "Contrasenia incorrecta.");
	  LCD_imprimir_L1(pagina);
	  LCD_Update();
  }
 // Reiniciar la longitud para permitir ingresar una nueva contraseña
 // longi = 0;		
}

void multiplex (void){
		if(i==1){
			//fila 1 0111
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_SET);  		//fila1
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET); 	//fila2		
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);	 //fila3	
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET);	 //fila4		
			FILA = 1;
		}
		if(i==2){
			//fila 2 1011
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_RESET);  //fila1
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET); 	 //fila2		
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);		 //fila3	
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET);		 //fila4		
			FILA = 2;
		}	
		if(i==3){
			//fila 3 1101
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_RESET);  //fila1
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET); 	 //fila2		
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_SET);		 //fila3	
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET);		 //fila4				
			FILA = 3;
		}
 		if(i==4){
			//fila 4 1110
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_RESET);  //fila1
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET); 	 //fila2		
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);		 //fila3	
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_SET);		 //fila4		
			FILA = 4;
		}
		i++;
		if(i==5) i=1;

}
char teclado_read(void){		
	tecla = teclas[FILA-1][COLUMNA-1];	
 return tecla;	
} //fin funcion

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//Envio un set al hilo 
	if((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1)){
				COLUMNA = 1;
			 }  //COL 1
		   else if((HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11) == 1)){
				COLUMNA = 2;
			 }  //COL 2
			 else if((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_10) == 1)){
				COLUMNA = 3;
			 }  //COL 3
			 else if((HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12) == 1)){
				COLUMNA = 4;
			 }  //COL 4				
  osThreadFlagsSet(tid_Thread,0x00000001U);  
}	
