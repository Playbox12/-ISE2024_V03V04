#include "LEDs.h"

void LED_Init(void) { //configuracion de los leds pin0=verde pin14=rojo
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void encender_LEDrojo(void){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
}

void encender_LEDverde(void){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
}

void apagar_LEDrojo(void){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
}

void apagar_LEDverde(void){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
}
