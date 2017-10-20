#include "CAN.h"
#include <stdio.h>
#include <stdint.h>
#include "menu.h"

unsigned int sysClock; // clockspeed in hz
volatile bool errFlag = 0; // transmission error flag
volatile bool rxFlag = 0; // msg recieved flag
unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes

tCANMsgObject msg[2];

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
	Init_Structs();
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


void CAN_Transmit(uint8_t data[4]){
	
	msgDataPtr[0] = data[0];
	msgDataPtr[1] = data[1];
	msgDataPtr[2] = data[2];
	msgDataPtr[3] = data[3];
	
	printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
	CANMessageSet(CAN0_BASE, 1, &msg[0], MSG_OBJ_TYPE_TX); // send as msg object 1
		
	if(errFlag) { // check for errors
		printf("CAN Bus Error\n");
	}
}

void Init_Structs(){
	msg[0].ui32MsgID = 1; // set up transmit message
  msg[0].ui32MsgIDMask = 0;
  msg[0].ui32Flags = MSG_OBJ_TX_INT_ENABLE; 
  msg[0].ui32MsgLen = sizeof(msgDataPtr);
  msg[0].pui8MsgData = msgDataPtr; 
	
	msg[1].ui32MsgID = 0; // set up receive message
  msg[1].ui32MsgIDMask = 0;
  msg[1].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  msg[1].ui32MsgLen = 8;
	msg[1].pui8MsgData = msgDataPtr; // set pointer to rx buffer
}







void Init_Receiver(){
	
	unsigned char msgData[8]; // 8 byte buffer for rx message data
	unsigned int data[4];

	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 1, &msg[1], MSG_OBJ_TYPE_RX);	
	
	while (1) {}
}	



void CAN_Slave(){
	unsigned char msgData[8]; // 8 byte buffer for rx message data
	unsigned int data[4];
	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 1, &msg[1], MSG_OBJ_TYPE_RX);
	printf("\n\nInitializing node as slave...\n");
	
	while(1) {
		if(rxFlag) { // rx interrupt has occured
			CANMessageGet(CAN0_BASE, 1, &msg[1], 0); // read CAN message object 1 from CAN peripheral	
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
			
			rxFlag = 0; // clear rx flag

			if(msg[1].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
				printf("CAN message loss detected\n");
			}
	    printf("Received colour\tr: %d  b: %d  g: %d  i: %d\n", msgData[0], msgData[1], msgData[2], msgData[3]);
			
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


