#include "project.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//=============================================================================================//
//	Jacob Clark
//	10/6/2016
//	Embedded Systems 
//	
//	This software is for the embedded systems lab midterm. It contains seven modules,
// a Systic, Uart_Helper, GPIO_init, PWM, CAN, ADC, and a Menu module. Some of them
// are always running, like the Uart, GPIO, Systic and Menu, while the others can be selected from
// the menu specifically. The NVIC is utilized without having a dedicated module.
// 
//	The CAN code contained in this software allows the user to select a board to function as 
// a reciever or transmitter. The transmitter board has two built-in transmission options, 
// but they can be editted. The CAN allows for a 32-bit data transmission per message. It is 
// set to operate at 0.5 Mbytes/sec. 
//
//	To interface with the software, you must use a serial port reading program such as 
// RealTerm. The data type should be set to Ansi at a baud rate of 38,400. Numeric data can
// be sent to the program to select menu options. 
//=============================================================================================//

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
		printf("Debugging interface initialized...\n");
		printf("GPIO ports initialized...\n");
		printf("Clock settings initialized...\n\n");
		printf("Set-up Complete.\n\n");
}