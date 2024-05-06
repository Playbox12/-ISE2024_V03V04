#include "cmsis_os2.h"

#ifndef __PWM_H
#define __PWM_H

#define MSGQUEUE_OBJECTS 16 
#define FREQ_TONO1 4000
#define FREQ_TONO2 6000
//MENSAJE
typedef struct {                                
  int CT;
} MSGQUEUE_OBJ_t;

extern osMessageQueueId_t mid_MsgQueuePWM;
//COLA
extern osMessageQueueId_t getModPWMQueueID(void);
int Init_MsgQueue_PWM (void);

//INICIAR THREAD PWM Y TEST
int Init_PWM (void);
int Init_PWM_Test(void);
void PWM_init(void);
void PWM_cambio(uint32_t frecuencia);
void PWM_desactivar();
void PWM_activar();
void clave_correcta();
void clave_erronea();
void alarma();
#endif
