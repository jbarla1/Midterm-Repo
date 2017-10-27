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
//  Main C File:
//=============================================================================================//

int  main(void) {
	SetupHardware();
	CAN_Init();
	
//	receive();
	transmit();
}	

void receive() {
	Init_Receiver();	
}

void transmit() {
	uint8_t data[4];
	uint8_t msgSelect;
	
	while(1){	
		printf("which message?\n");
		msgSelect = getchar();
		
		data[0] = 0;
		data[1] = 0;
		data[2] = 128; 
		data[3] = 128;		
		
		printf("sending message\n");
		CAN_Transmit(data, msgSelect);

		printf("send again new data on same message?\n");
		getchar(); 

		data[0] = 0;
		data[1] = 128;
		data[2] = 0; 
		data[3] = 128;			

		printf("sending message\n");
		CAN_Transmit(data, msgSelect);
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