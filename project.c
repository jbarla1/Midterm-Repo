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
//  Main C File: This CAN code is functional with 7 receive and transmit objects.
//=============================================================================================//



int  main(void) {
	setupHardware();

  trial_recieve();	
	
//	transmit();
}	




void trial_recieve(){
	initReceiver();
	while(1){
		printf("Display data?");
		getchar();
		printf("MsgData[0] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[0][0], MsgData[0][1], MsgData[0][2], MsgData[0][3]);
		printf("MsgData[1] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[1][0], MsgData[1][1], MsgData[1][2], MsgData[1][3]);
		printf("MsgData[2] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[2][0], MsgData[2][1], MsgData[2][2], MsgData[2][3]);
		printf("MsgData[3] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[3][0], MsgData[3][1], MsgData[3][2], MsgData[3][3]);	
		printf("MsgData[4] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[4][0], MsgData[4][1], MsgData[4][2], MsgData[4][3]);
		printf("MsgData[5] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[5][0], MsgData[5][1], MsgData[5][2], MsgData[5][3]);
		printf("MsgData[6] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[6][0], MsgData[6][1], MsgData[6][2], MsgData[6][3]);
	}
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

void setupHardware(){
		ClockSetup();
		GPIO_Setup();
		UartSetup();
	  CAN_Init();
		printf("Debugging interface initialized...\n");
		printf("GPIO ports initialized...\n");
		printf("Clock settings initialized...\n\n");
		printf("Set-up Complete.\n\n");
}


void displayData(){

}