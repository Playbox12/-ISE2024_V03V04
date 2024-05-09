#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stdio.h"
#include "lcd.h" 
#include "stm32f4xx_hal.h"
#include "string.h"
#include "Thread_Teclado.h"

/*----------------------------------------------------------------------------
 *     											 Thread TECLADO
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Teclado;                        // thread id

void Thread (void *argument);                   // thread function
uint8_t secuencia;
char pagina[100];
uint8_t FILA,COLUMNA;
extern osTimerId_t tim_50ms;                            // timer id
char tecla;
char teclado_read(void); 
void multiplex (void);
void guardar_pssw(char tecla);
char pssw_T[MAX_LONGITUD_CONTRASENIA+1]; // +1 para el terminador nulo ('\0')
int longi=0;
extern char pssw_S[5];
	
char teclas [4][4] = {{'1','2','3','A'},
											 {'4','5','6','B'},
											 {'7','8','9','C'},
											 {'*','0','#','D'}};
int i,j;
											 
int Init_Thread_T (void) {
 
  tid_Teclado = osThreadNew(Thread, NULL, NULL);
  if (tid_Teclado == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {	
		//osTimerStart(tim_50ms,200U); //para los rebotes
    multiplex();
		secuencia=osThreadFlagsWait(0x00000002U,osFlagsWaitAny,50);
     if(secuencia==0x00000002U){
      	sprintf(pagina, "pulsado");
			  writeline1(pagina);
	      LCD_update();	
			  longi++;
			  tecla = teclado_read();		 	
	      writeline2(&tecla);
	      LCD_update();
			  guardar_pssw(tecla);
     }
		}
 }

void guardar_pssw(char tecla){
	
	if(longi == 1)
		pssw_T[0] = tecla;
	if(longi == 2)
		pssw_T[1] = tecla;
	if(longi == 3)
		pssw_T[2] = tecla;
	if(longi == 4)
		pssw_T[3] = tecla;	
	
	if(longi == 4){	 
		 writeline1(pssw_T);
	   LCD_update();
	}
	
 // Comparar la contraseña con otra cadena
  if ((longi == 4) & (strcmp(pssw_T, pssw_S) == 0)){ // Cambia "1234" por la contraseña a comparar   
	 
		sprintf(pagina, "Contrasenia correcta");
	  writeline1(pagina);
	  LCD_update();
	}	
  if((longi == 4) & (strcmp(pssw_T, pssw_S) != 0)){
		sprintf(pagina, "Contrasenia incorrecta.");
	  writeline1(pagina);
	  LCD_update();
  }
		
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


