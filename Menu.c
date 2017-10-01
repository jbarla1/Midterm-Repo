#include "menu.h"
//extern char inputChar2;
void SelectionMenu() {
		char inputChar1, inputChar2;
	 	printf("To flash an onboard LED enter 1. To enable CAN functionality enter 2. To display PWM on onboard LED enter 3.");
		inputChar1 = getchar();
	
		switch(inputChar1) {
			case 1:
				LedMenu();	
			break; 
			case 2:
			CAN_Init();	
			
			printf("Enter 1 to set board as master, or 0 for slave.");
			inputChar2 = getchar();
			if(inputChar2){
				CAN_Master();
			}
			else{
				CAN_Slave();
			}
			break;
			case 3:
				PWM_Setup();
				printf("PWM Initialized");
				pulse();
			break;
		
		}
		printf("Would you like to flash an LED on the slave board through the CAN?\n (1 = Yes, 0 = No\n");
	  
		if(inputChar2){
			CAN_Master();
		}
}	




void LedMenu() {
		char Char1, Char2, Char3;

		printf("Red, blue, or green? Enter 1, 2, or 3 respectively.\n");
		Char1 = getchar();
	  printf("How many times should it blink per second?\n");
		Char2 = getchar();
		printf("For how many seconds?\n");
		Char3 = getchar();
	
		switch(Char1){
					case 1:
					
					while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
					delayMS(1000/Char2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
					delayMS(1000/Char2);
					}
										
					case 2:
					while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xF);
					delayMS(1000/Char2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
					delayMS(1000/Char2);

					}
					case 3:
						while(1){
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
					delayMS(1000/Char2);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
					delayMS(1000/Char2);
					
						}
					default:
						
					printf("Sorry, that wasnt an option");
				
					break;
				}
}