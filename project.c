#include "project.h"
#include <stdio.h>
#include <stdint.h>
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


//==============================================================//
//==============================================================//
//==============================================================//
//==============================================================//




int  main(void) {
		char inputChar1;
		uint32_t inputChar2;
		SetupHardware();
			
	 	printf("Would you like to flash an LED of ANY color you want!? \nOf course you do, but just to be sure, enter a one if so, and a zero if not.\n");
		inputChar1 = getchar();
		
		if(inputChar1){
				
				printf("Red, blue, or green? Enter 1, 2, or 3 respectively.\n");
				inputChar1 = getchar();
			  printf("How many times should it blink per second?\n");
				inputChar2 = getchar();
				
				switch(inputChar1){
					case 1:
					
					while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
					delayMS(1000/inputChar2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
					delayMS(1000/inputChar2);
					}
										
					case 2:
					while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xF);
					delayMS(1000/inputChar2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
					delayMS(1000/inputChar2);

					}
					case 3:
						while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
					delayMS(1000/inputChar2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
					delayMS(1000/inputChar2);
					
						}
					default:
						
					printf("sorry, that wasnt an option");
				
					break;
				
			
				}
		}
	//	while(1){
	//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
	//		delayMS(1000);
	//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
	//		delayMS(1000);
	//	}
}


void SetupHardware(){
		ClockSetup();
		GPIO_Setup();
		UartSetup();	
	//	PWM_Setup();
}
