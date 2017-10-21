#include "CAN.h"
#include <stdio.h>
#include <stdint.h>
#include "menu.h"

unsigned int sysClock; // clockspeed in hz
volatile bool errFlag = 0; // transmission error flag
volatile bool rxFlag = 0; // msg recieved flag

unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes
unsigned char RxMsgData[8]; // 8 byte buffer for rx message data

unsigned int msgData2; // the message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr2 = (unsigned char *)&msgData2; // make a pointer to msgData so we can access individual bytes
unsigned char RxMsgData2[8];

unsigned char MsgData[2][8]; //eventually will be external message data 

tCANMsgObject TxObj[2], RxObj[2]; 

// ------------------------------------------------------
//			CAN Initialize function:
// ------------------------------------------------------
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
// ------------------------------------------------------
//			CAN Interrupt Handler:
// -----------------------------------------------------

// try using CAN_STS_NEWDAT to get data about which Rx Object generated an interrupt
// CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT);

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
		CANMessageGet(CAN0_BASE, 1, &RxObj[0], false);	
		
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
		CANMessageGet(CAN0_BASE, 3, &RxObj[1], false);
	}
	else { // should never happen
		printf("Unexpected CAN bus interrupt\n");
	}
}
// ------------------------------------------------------
//			CAN Transmit function:
// ------------------------------------------------------
void CAN_Transmit(uint8_t data[4], uint8_t msgSelect){
	if(msgSelect==1){
		msgDataPtr[0] = data[0];
		msgDataPtr[1] = data[1];
		msgDataPtr[2] = data[2];
		msgDataPtr[3] = 1;
		printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
		CANMessageSet(CAN0_BASE, 1, &TxObj[0], MSG_OBJ_TYPE_TX); // send as msg object 1
	}
	else if(msgSelect==2){
		msgDataPtr2[0] = data[0];
		msgDataPtr2[1] = data[1];
		msgDataPtr2[2] = data[2];
		msgDataPtr2[3] = 2;
		printf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr2[0], msgDataPtr2[1], msgDataPtr2[2]); // write colour to UART for debugging	
		CANMessageSet(CAN0_BASE, 2, &TxObj[1], MSG_OBJ_TYPE_TX); // send as msg object 2
	}
	if(errFlag) { // check for errors
		printf("CAN Bus Error\n");
	}
}
// ------------------------------------------------------
//			CAN Receive function:
// ------------------------------------------------------
void Init_Receiver(){

	CANMessageSet(CAN0_BASE, 1, &RxObj[0], MSG_OBJ_TYPE_RX);	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 2, &RxObj[1], MSG_OBJ_TYPE_RX);	
	
	while (1) {		
		if(rxFlag){		  
			printf("B1 Received colour\tr: %d  b: %d  g: %d  i: %d\n", MsgData[0][0], MsgData[0][1], MsgData[0][2], MsgData[0][3]);
			printf("B2 Received colour\tr: %d  b: %d  g: %d  i: %d\n", MsgData[1][0], MsgData[1][1], MsgData[1][2], MsgData[1][3]);
			rxFlag=0;
		}
		if(RxObj[0].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
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
// ------------------------------------------------------
//						Struct Inits:
// ------------------------------------------------------
void Init_Structs(){ // last working message structs (receives both messages)
	TxObj[0].ui32MsgID = 0x03; 
  TxObj[0].ui32MsgIDMask = 0x00;
  TxObj[0].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;  
  TxObj[0].ui32MsgLen = sizeof(msgDataPtr);
  TxObj[0].pui8MsgData = msgDataPtr; 
	
	RxObj[0].ui32MsgID = 0x03; // this receive ID acts as the filter 
  RxObj[0].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  RxObj[0].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  RxObj[0].ui32MsgLen = 8;
	RxObj[0].pui8MsgData = RxMsgData; // This pointer needs to be set to buffer for Rx Message data. 
	
	TxObj[1].ui32MsgID = 0x02; 
  TxObj[1].ui32MsgIDMask = 0x00;
  TxObj[1].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER; 
  TxObj[1].ui32MsgLen = sizeof(msgDataPtr2);
  TxObj[1].pui8MsgData = msgDataPtr2; 
	
//  RxObj[1].ui32MsgID = 0x02; // this receive ID acts as the filter 
//  RxObj[1].ui32MsgIDMask = 0x00; // mask to enable filtration bits
//  RxObj[1].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
//  RxObj[1].ui32MsgLen = 8;
//	RxObj[1].pui8MsgData = RxMsgData2; // This pointer needs to be set to buffer for Rx Message data. 
}
// ------------------------------------------------------
void Init_Structs1(){ // this case has no filter, just always uses identical MsgId's for Tx and Rx
	TxObj[0].ui32MsgID = 0x01; 
  TxObj[0].ui32MsgIDMask = 0x00;
  TxObj[0].ui32Flags = MSG_OBJ_TX_INT_ENABLE;  
  TxObj[0].ui32MsgLen = sizeof(msgDataPtr);
  TxObj[0].pui8MsgData = msgDataPtr; 
	
	TxObj[1].ui32MsgID = 0x02; 
  TxObj[1].ui32MsgIDMask = 0x00;
  TxObj[1].ui32Flags = MSG_OBJ_TX_INT_ENABLE; 
  TxObj[1].ui32MsgLen = sizeof(msgDataPtr2);
  TxObj[1].pui8MsgData = msgDataPtr2;  
	
	RxObj[0].ui32MsgID = 0x01; // this receive ID acts as the filter 
  RxObj[0].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  RxObj[0].ui32Flags = MSG_OBJ_RX_INT_ENABLE;
  RxObj[0].ui32MsgLen = 8;
	RxObj[0].pui8MsgData = RxMsgData; // This pointer needs to be set to buffer for Rx Message data. 
	
  RxObj[1].ui32MsgID = 0x02; // this receive ID acts as the filter 
  RxObj[1].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  RxObj[1].ui32Flags = MSG_OBJ_RX_INT_ENABLE;
  RxObj[1].ui32MsgLen = 8;
	RxObj[1].pui8MsgData = RxMsgData2; // This pointer needs to be set to buffer for Rx Message data. 
}
// ------------------------------------------------------ 
void Init_Structs2(){ // all use filter, both should reject and accept their corresponding message
	TxObj[0].ui32MsgID = 0x0F; 
  TxObj[0].ui32MsgIDMask = 0xFF;
  TxObj[0].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;  
  TxObj[0].ui32MsgLen = sizeof(msgDataPtr);
  TxObj[0].pui8MsgData = msgDataPtr; 
	
	TxObj[1].ui32MsgID = 0xF0; 
  TxObj[1].ui32MsgIDMask = 0xFF;
  TxObj[1].ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER; 
  TxObj[1].ui32MsgLen = sizeof(msgDataPtr2);
  TxObj[1].pui8MsgData = msgDataPtr2;  
	
	RxObj[0].ui32MsgID = 0x0F; // this receive ID acts as the filter 
  RxObj[0].ui32MsgIDMask = 0xFF; // mask to enable filtration bits
  RxObj[0].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  RxObj[0].ui32MsgLen = 8;
	RxObj[0].pui8MsgData = RxMsgData; // This pointer needs to be set to buffer for Rx Message data. 
	
  RxObj[1].ui32MsgID = 0xF0; // this receive ID acts as the filter 
  RxObj[1].ui32MsgIDMask = 0xFF; // mask to enable filtration bits
  RxObj[1].ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  RxObj[1].ui32MsgLen = 8;
	RxObj[1].pui8MsgData = RxMsgData2; // This pointer needs to be set to buffer for Rx Message data. 
}
// ------------------------------------------------------
//
// ------------------------------------------------------