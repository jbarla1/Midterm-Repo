#include "CAN.h"
#include <stdio.h>
#include <stdint.h>
#include "menu.h"

unsigned int sysClock; // clockspeed in hz
volatile bool errFlag = 0; // transmission error flag
volatile bool rxFlag = 0; // msg recieved flag

void CAN_Init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	printf("\nInitializing CAN0RX...\n");
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	printf("Initializing CAN0TX...\n");
	GPIOPinConfigure(GPIO_PE5_CAN0TX);
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	printf("Initializing CAN0_BASE...\n");
	CANInit(CAN0_BASE);
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
	printf("Setting CAN BitRate: 0.5 Megabytes/sec...\n");
	CANIntRegister(CAN0_BASE, CANIntHandler); // use dynamic vector table allocation
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	CANEnable(CAN0_BASE);
	printf("CAN Initialized.\n\n");
}

void CANIntHandler(void) {
	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status

	if(status == CAN_INT_INTID_STATUS) { // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		errFlag = 1;
	} 
	
	else if(status == 1) { // message object 1
		CANIntClear(CAN0_BASE, 1); // clear interrupt
	  rxFlag = 1; // set rx flag
		errFlag = 0; // clear any error flags
	}
	
	else { // should never happen
		printf("Unexpected CAN bus interrupt\n");
	}
}



void CAN_Master(void) {
	tCANMsgObject msg; // the CAN message object
	unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
	unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes
	uint32_t x = 20;
	bool input;
	char choice;
	uint8_t loop =0;
	
	msgData = 0;
	msg.ui32MsgID = 1;
	msg.ui32MsgIDMask = 0;
	msg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
	msg.ui32MsgLen = sizeof(msgDataPtr);
	msg.pui8MsgData = msgDataPtr;
	
	printf("Would you like to flash colors or select colors?\n Enter a 1 for flashing or 0 for selecting");
	input = getchar();

	if(input==1){
		while(1) {
		
			loop++;

			if(loop==4){
				loop = 1;
			}
			switch (loop) {
				case 1:
					msgDataPtr[0] = 128;
					msgDataPtr[1] = 0;
					msgDataPtr[2] = 0;
					msgDataPtr[3] = 128;
				break;
				case 2:
					msgDataPtr[0] = 0;
					msgDataPtr[1] = 128;
					msgDataPtr[2] = 0;
					msgDataPtr[3] = 128;
				break;	
				case 3:
					msgDataPtr[0] = 0;
					msgDataPtr[1] = 0;
					msgDataPtr[2] = 128;
					msgDataPtr[3] = 128;
				break;
			}
			if(x == 100){
				x = 20;
			}
			printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
			CANMessageSet(CAN0_BASE, 1, &msg, MSG_OBJ_TYPE_TX); // send as msg object 1		
			
			delayMS(x); // wait 100ms
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
			delayMS(x);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
			x++;	
			if(errFlag) { // check for errors
				printf("CAN Bus Error\n");
			}
		}
	}
	if(input==0){
		while(1){
			printf("Red, blue, or green? Enter 1, 2, or 3 respectively.");
			choice = getchar();
			switch(choice){
				case 1:
					msgDataPtr[0] = 128;
					msgDataPtr[1] = 0;
					msgDataPtr[2] = 0;
					msgDataPtr[3] = 128;
				break;
				case 2:
					msgDataPtr[0] = 0;
					msgDataPtr[1] = 128;
					msgDataPtr[2] = 0;
					msgDataPtr[3] = 128;
				break;
		
				case 3:
					msgDataPtr[0] = 0;
					msgDataPtr[1] = 0;
					msgDataPtr[2] = 128;
					msgDataPtr[3] = 128;
				break;
			}
			
			printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
			CANMessageSet(CAN0_BASE, 1, &msg, MSG_OBJ_TYPE_TX); // send as msg object 1
		
			if(errFlag) { // check for errors
				printf("CAN Bus Error\n");
			}
		}	
	}
}



void CAN_Slave(){

  volatile uint32_t ui32Loop;
	tCANMsgObject msg; // the CAN msg object
	unsigned char msgData[8]; // 8 byte buffer for rx message data

	msg.ui32MsgID = 0;
	msg.ui32MsgIDMask = 0;
	msg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
	msg.ui32MsgLen = 8; // allow up to 8 bytes

	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 1, &msg, MSG_OBJ_TYPE_RX);
 
	unsigned int colour[3];
	float intensity;
	
	while(1) {
		if(rxFlag) { // rx interrupt has occured
			msg.pui8MsgData = msgData; // set pointer to rx buffer
			CANMessageGet(CAN0_BASE, 1, &msg, 0); // read CAN message object 1 from CAN peripheral
			
			
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
			
			rxFlag = 0; // clear rx flag

			if(msg.ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
				printf("CAN message loss detected\n");
			}
			
			colour[0] = msgData[0] * 0xFF;
			colour[1] = msgData[1] * 0xFF;
			colour[2] = msgData[2] * 0xFF;
			intensity = msgData[3] / 255.0f; // scale from 0-255 to float 0-1

			
	    printf("Received colour\tr: %d  g: %d  b: %d  i: %d\n", msgData[0], msgData[1], msgData[2], msgData[3]);

			// set colour and intensity
			if (msgData[0] == 128){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
			}
			if (msgData[1] == 128){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xF);
			}
			if (msgData[2] == 128){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
			}
		}
	}
}