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

unsigned char MsgData[NUM_RX_MESSAGES][4];

int  main(void) {
	SetupHardware();
	CAN_Init();
	
//	receive();
//  trial_recieve();	
	
	
	transmit();
}	

void receive() {
	Steady_Receiver();	
}

void trial_recieve(){

Init_receiver();

	while(1){
	printf("Display data?");
	getchar();
		
	
		  printf("MsgData[0] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[0][0], MsgData[0][1], MsgData[0][2], MsgData[0][3]);
			printf("MsgData[1] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[1][0], MsgData[1][1], MsgData[1][2], MsgData[1][3]);
			printf("MsgData[2] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[2][0], MsgData[2][1], MsgData[2][2], MsgData[2][3]);
			printf("MsgData[3] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[3][0], MsgData[3][1], MsgData[3][2], MsgData[3][3]);
		
		
		
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

void SetupHardware(){
		ClockSetup();
		GPIO_Setup();
		UartSetup();
		printf("Debugging interface initialized...\n");
		printf("GPIO ports initialized...\n");
		printf("Clock settings initialized...\n\n");
		printf("Set-up Complete.\n\n");
}