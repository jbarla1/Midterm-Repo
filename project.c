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

int  main(void) {
	bool choice = 1;
	SetupHardware();

	while(choice) {
		SelectionMenu(); 	
			
		printf("Would you like to return to the menu or terminate the program? (1=continue, 0=terminate)");
		choice = getchar();
	}		
}



void SetupHardware(){
		ClockSetup();
		GPIO_Setup();
		UartSetup();
		printf("Debugging interface Initialized...\n\n");
		printf("Set-up Complete.\n\n");
}