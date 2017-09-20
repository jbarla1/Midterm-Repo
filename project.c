#include "project.h"

#include <stdio.h>
#include <stdint.h>
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif





int  main(void) {
	


	
	    SetupHardware();
			
	while(1){
		
		 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
		 delayMS(1000);
		 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
		 delayMS(1000);
	
	

}

}


void SetupHardware(){
	ClockSetup();
	GPIO_Setup();
	UartSetup();	
	//PWM_Setup();
}
