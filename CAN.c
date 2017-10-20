#include "CAN.h"
#include <stdio.h>
#include <stdint.h>
#include "menu.h"

unsigned int sysClock; // clockspeed in hz
volatile bool errFlag = 0; // transmission error flag
volatile bool rxFlag = 0; // msg recieved flag
unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes

unsigned int msgData2; // the message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr2 = (unsigned char *)&msgData2; // make a pointer to msgData so we can access individual bytes

unsigned char RxMsgData[8]; // 8 byte buffer for rx message data

unsigned char MsgData[2][8];

tCANMsgObject msg[4];

void Init_Structs(){
	msg[0].ui32MsgID = 0x03; // set up transmit message
  msg[0].ui32MsgIDMask = 0x00;
  msg[0].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;  
  msg[0].ui32MsgLen = sizeof(msgDataPtr);
  msg[0].pui8MsgData = msgDataPtr; 
	
	msg[1].ui32MsgID = 0x03; // this receive ID acts as the filter 
  msg[1].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  msg[1].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  msg[1].ui32MsgLen = 8;
	msg[1].pui8MsgData = RxMsgData; // This pointer needs to be set to buffer for Rx Message data. 
	
	msg[2].ui32MsgID = 0x02; // set up transmit message 2
  msg[2].ui32MsgIDMask = 0x00;
  msg[2].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER; 
  msg[2].ui32MsgLen = sizeof(msgDataPtr2);
  msg[2].pui8MsgData = msgDataPtr2; 
	
//  msg[3].ui32MsgID = 0x02; // this receive ID acts as the filter 
//  msg[3].ui32MsgIDMask = 0x00; // mask to enable filtration bits
//  msg[3].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
//  msg[3].ui32MsgLen = 8;
//	msg[3].pui8MsgData = RxMsgData2; // This pointer needs to be set to buffer for Rx Message data. 
}

void CAN_Init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
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
	printf("Interrupt occurred\n");
	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status
	
	printf("status: %d\n", status);
	
	if(status == CAN_INT_INTID_STATUS) { // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		errFlag = 1;
		printf("test0\n");
	} 
	else if(status == 1) { 
		CANIntClear(CAN0_BASE, 1); // clear interrupt	
	  rxFlag = 1; // set rx flag
		errFlag = 0; // clear any error flags	
		printf("test1\n");
		CANMessageGet(CAN0_BASE, 1, &msg[1], false);	
		
		printf("B1 Received colour\tr: %d  b: %d  g: %d  i: %d\n", RxMsgData[0], RxMsgData[1], RxMsgData[2], RxMsgData[3]);
		
		if(RxMsgData[3]==1){
			MsgData[0][0] = RxMsgData[0];
			MsgData[0][1] = RxMsgData[1];
			MsgData[0][2] = RxMsgData[2];
			MsgData[0][3] = RxMsgData[3];
		}
	  else if(RxMsgData[3]==2){
			MsgData[1][0] = RxMsgData[0];
			MsgData[1][1] = RxMsgData[1];
			MsgData[1][2] = RxMsgData[2];
			MsgData[1][3] = RxMsgData[3];
		}
		
	}
	else if(status == 2) { 
		CANIntClear(CAN0_BASE, 2);
	  rxFlag = 1; // set rx flag
		errFlag = 0; // clear any error flags	
		printf("test2\n");
		CANMessageGet(CAN0_BASE, 3, &msg[3], false);
	}
	else { // should never happen
		printf("Unexpected CAN bus interrupt\n");
	}
}

void CAN_Transmit(uint8_t data[4], uint8_t msgSelect){
	if(msgSelect==1){
		msgDataPtr[0] = data[0];
		msgDataPtr[1] = data[1];
		msgDataPtr[2] = data[2];
		msgDataPtr[3] = 1;
		printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
		CANMessageSet(CAN0_BASE, 1, &msg[0], MSG_OBJ_TYPE_TX); // send as msg object 1
	}
	else if(msgSelect==2){
		msgDataPtr2[0] = data[0];
		msgDataPtr2[1] = data[1];
		msgDataPtr2[2] = data[2];
		msgDataPtr2[3] = 2;
		printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr2[0], msgDataPtr2[1], msgDataPtr2[2]); // write colour to UART for debugging	
		CANMessageSet(CAN0_BASE, 2, &msg[2], MSG_OBJ_TYPE_TX); // send as msg object 2
	}
	if(errFlag) { // check for errors
		printf("CAN Bus Error\n");
	}
}


void Init_Receiver(){

	
	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 1, &msg[1], MSG_OBJ_TYPE_RX);	
	//CANMessageSet(CAN0_BASE, 2, &msg[3], MSG_OBJ_TYPE_RX);	
	
	while (1) {
		
		if(rxFlag){		  
				printf("B1 Received colour\tr: %d  b: %d  g: %d  i: %d\n", MsgData[0][0], MsgData[0][1], MsgData[0][2], MsgData[0][3]);
				printf("B2 Received colour\tr: %d  b: %d  g: %d  i: %d\n", MsgData[1][0], MsgData[1][1], MsgData[1][2], MsgData[1][3]);
				rxFlag=0;
		}
			if(msg[1].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
				printf("CAN message loss detected\n");
			}
			if (MsgData[0][0] == 128){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
			}
			if (MsgData[0][1] == 128){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xF);
			}
			if (MsgData[0][2] == 128){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xF);
		}
	}
}	






