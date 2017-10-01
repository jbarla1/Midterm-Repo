#include "menu.h"

void SelectionMenu() {
		char inputChar1, inputChar2;
	 	printf("Would you like to flash an LED on this board?\n (1 = Yes, 0 = No)\n");
		inputChar1 = getchar();
	
		if(inputChar1){
				LedMenu();	
		}
		printf("Would you like to flash an LED on the slave board through the CAN?\n (1 = Yes, 0 = No\n");
	  inputChar2 = getchar();
		if(inputChar2){
			CAN_Set();
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