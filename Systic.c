#include "Systic.h"


#ifdef DEBUG
void__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

void SysTickEnable(void);
void SysTickPeriodSet(uint32_t ui32Period);
uint32_t SysTickValueGet(void);
void SysCtlClockSet(uint32_t ui32Config);




void ClockSetup() {
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysTickEnable(); 
	SysTickPeriodSet(16000000); 
}


void delay(uint32_t duration)
{
	uint32_t time =0;
	printf("waiter");


while (time < duration){
	printf("waiting");
	time = SysTickValueGet();	
	}

 }

 void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}
 




//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
	//		delayMS(1000);
	//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
	//		delayMS(1000);