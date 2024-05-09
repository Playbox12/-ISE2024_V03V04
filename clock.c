#include "clock.h"

 
osThreadId_t tid_Thclock;                        // thread id 

char hora[80];
char fecha[80];

void Thclock (void *argument);                   // thread function
 
bool get_time (void) {
	bool aux=false;
//	netSNTPc_GetTime (NULL, time_cback);
	if (netSNTPc_GetTime (NULL, time_cback) == netOK){
		aux=true;
	}else
		aux=false;
	
	return aux;
}
 
static void time_cback (uint32_t seconds, uint32_t seconds_fraction) {
		
		RTC_TimeTypeDef stimestructureweb;
		RTC_DateTypeDef sdatestructureweb;
	
		time_t rawtime = seconds;
    struct tm  ts;
    char buf[80];	

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
		
		sdatestructureweb.Year=(buf[6] - '0') * 10 + (buf[7] - '0');
		sdatestructureweb.Month=(buf[9] - '0') * 10 + (buf[10] - '0');
		sdatestructureweb.Date=(buf[12] - '0') * 10 + (buf[13] - '0');
	
		stimestructureweb.Hours = ts.tm_hour;//(buf[15] - '0') * 10 + (buf[16] - '0');
		stimestructureweb.Minutes = ts.tm_min;//(buf[18] - '0') * 10 + (buf[19] - '0');
		stimestructureweb.Seconds = ts.tm_sec;//(buf[21] - '0') * 10 + (buf[22] - '0');
	
		HAL_RTC_SetTime(&hrtc,&stimestructureweb,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&sdatestructureweb,RTC_FORMAT_BIN);
		
		
}

void RTC_Initialize(void){
	
	hrtc.Instance=RTC;
	hrtc.Init.HourFormat=RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv=0x7F;
	hrtc.Init.SynchPrediv=0x00FF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&hrtc);
	
	HAL_RTC_Init(&hrtc);
	calendar_config();
	

}

void calendar_config(void){
	
	RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
	
	sdatestructure.Year = 0x23;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
	
	HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD);
	
	stimestructure.Hours=14;
	stimestructure.Minutes=32;
	stimestructure.Seconds=57;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
	
	HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BIN);
	
	
}

void Escribir_hora_rtc(void){
	
	uint8_t linea1[80];
	uint8_t linea2[80];
	
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	
	HAL_RTC_GetDate(&hrtc,&sdatestructureget,RTC_FORMAT_BIN);
	sprintf((char *)linea2,"Fecha: %2.2d-%2.2d-%2.2d",sdatestructureget.Date, sdatestructureget.Month, 2000 + sdatestructureget.Year);
	HAL_RTC_GetTime(&hrtc,&stimestructureget,RTC_FORMAT_BIN);
	sprintf((char *)linea1,"Hora: %2.2d:%2.2d:%2.2d",stimestructureget.Hours,stimestructureget.Minutes,stimestructureget.Seconds);
	
	//writeline1((char *)linea1);
	//writeline2((char *)linea2);
	//LCD_update();
	
	sprintf(hora,"Hora: %2.2d:%2.2d:%2.2d", stimestructureget.Hours,stimestructureget.Minutes,stimestructureget.Seconds);
	sprintf(fecha,"F: %2.2d-%2.2d-%2.2d",sdatestructureget.Date, sdatestructureget.Month, 2000 + sdatestructureget.Year);
}

int Init_Thclock (void) {
 
  tid_Thclock = osThreadNew(Thclock, NULL, NULL);
  if (tid_Thclock == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thclock (void *argument) {
	
	RTC_Initialize();

	 get_time();
  while (1) {
		Escribir_hora_rtc();
	
  }
}

