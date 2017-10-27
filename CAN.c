#include "CAN.h"
#include <stdio.h>
#include <stdint.h>
// --------------------------------------------------------------------------------------
//			Global variables and structs:
// --------------------------------------------------------------------------------------
volatile bool errFlag = 0;                                                       // Transmission error flag
volatile bool rxFlag = 0;                                                        // Msg recieved flag

unsigned int msgData;                                                            // Four bytes of message data allocated as uint32
unsigned char *msgDataPtr = (unsigned char *)&msgData;                           // Pointer to message data for individual byte access
unsigned char RxMsgData[8];                                                      // 8 byte buffer for RxObj[0] message data

unsigned int msgData2;                                                           // The message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr2 = (unsigned char *)&msgData2;                         // Make a pointer to msgData so we can access individual bytes
unsigned char RxMsgData2[8];                                                     // 8 byte buffer for RxObj[1] message data

unsigned int msgData3;                                                           // The message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr3 = (unsigned char *)&msgData3;                         // Make a pointer to msgData so we can access individual bytes
unsigned char RxMsgData3[8];                                                     // 8 byte buffer for RxObj[1] message data

unsigned int msgData4;                                                           // The message data is four bytes long which we can allocate as an int32
unsigned char *msgDataPtr4 = (unsigned char *)&msgData4;                         // Make a pointer to msgData so we can access individual bytes
unsigned char RxMsgData4[8];                                                     // 8 byte buffer for RxObj[1] message data

unsigned char MsgData[NUM_RX_MESSAGES][4];                                   		 // Eventually will be external message data for Rx messages

tCANMsgObject TxObj[NUM_TX_MESSAGES], RxObj[NUM_RX_MESSAGES];                    // Set up Tx and Rx TCANMsgObject structs with defined quantites
// --------------------------------------------------------------------------------------
//			CAN initialize function:
// --------------------------------------------------------------------------------------
void CAN_Init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                                   // Enable GPIO for port E for use on CAN
	GPIOPinConfigure(GPIO_PE4_CAN0RX);                                             // Assign pin E4 to CAN0RX
	GPIOPinConfigure(GPIO_PE5_CAN0TX);                                             // Assign pin E5 to CAN0TX
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);                      // Enable pin type for CAN
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);                                    // Enable CAN periph
	printf("Initializing CAN0_BASE...\n");                                         // Printout for debugging
	CANInit(CAN0_BASE);                                                            // Initialize CAN0 base
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);                            // Set CAN bitrate
	printf("Setting CAN BitRate: 0.5 Megabytes/sec...\n");                         // Printout for debugging
	CANIntRegister(CAN0_BASE, CANIntHandler);                                      // Give dynamic vector table name of handler for CAN Interrupts
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);      // Enable CAN interrupts
	IntEnable(INT_CAN0);									                                         // Enable global interrupts
	CANEnable(CAN0_BASE);                                                          // Enable CAN functionality
	printf("CAN Initialized.\n\n");                                                // Printout for debugging
	Init_Structs();                                                                // Call function to initialize message structs
}
// --------------------------------------------------------------------------------------
//			CAN interrupt handler:
// --------------------------------------------------------------------------------------
void CANIntHandler(void) {

	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status cause
	printf("Interrupt status id: %d\n", status);
	
	if(status == CAN_INT_INTID_STATUS) {                           // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		errFlag = 1;
	} 
	else if(status == 1) { 
		CANIntClear(CAN0_BASE, 1); // clear interrupt	
		errFlag = 0; // clear any error flags	
		CANMessageGet(CAN0_BASE, 1, &RxObj[0], false);
		rxFlag=0;
		
		MsgData[0][0]=RxMsgData[0];
		MsgData[0][1]=RxMsgData[1];
		MsgData[0][2]=RxMsgData[2];
		MsgData[0][3]=RxMsgData[3];
		
	}
	else if(status == 2) { 
		CANIntClear(CAN0_BASE, 2); // clear interrupt	
		errFlag = 0; // clear any error flags	
		CANMessageGet(CAN0_BASE, 2, &RxObj[1], false);
		
		MsgData[1][0]=RxMsgData2[0];
		MsgData[1][1]=RxMsgData2[1];
		MsgData[1][2]=RxMsgData2[2];
		MsgData[1][3]=RxMsgData2[3];
		
		rxFlag=0;		
	}
		else if(status == 3) { 
		CANIntClear(CAN0_BASE, 3); // clear interrupt	
		errFlag = 0; // clear any error flags	
		CANMessageGet(CAN0_BASE, 3, &RxObj[2], false);
		
		MsgData[2][0]=RxMsgData3[0];
		MsgData[2][1]=RxMsgData3[1];
		MsgData[2][2]=RxMsgData3[2];
		MsgData[2][3]=RxMsgData3[3];		
			
		rxFlag=0;		
	}
	else if(status == 4) { 
	CANIntClear(CAN0_BASE, 4); // clear interrupt	
	errFlag = 0; // clear any error flags	
	CANMessageGet(CAN0_BASE, 4, &RxObj[3], false);	
	
		MsgData[3][0]=RxMsgData4[0];
	  MsgData[3][1]=RxMsgData4[1];
		MsgData[3][2]=RxMsgData4[2];
		MsgData[3][3]=RxMsgData4[3];
		
	rxFlag=1;
	}	
	else if(status == 5) { 
		CANIntClear(CAN0_BASE, 5); // clear interrupt	
		errFlag = 0; // clear any error flags		
	}	
	else if(status == 6) { 
		CANIntClear(CAN0_BASE, 6); // clear interrupt	
		errFlag = 0; // clear any error flags		
	}	
		else if(status == 7) { 
		CANIntClear(CAN0_BASE, 7); // clear interrupt	
		errFlag = 0; // clear any error flags		
	}	
		else if(status == 8) { 
		CANIntClear(CAN0_BASE, 8); // clear interrupt	
		errFlag = 0; // clear any error flags		
	}	
	else { // should never happen
		printf("Unexpected CAN bus interrupt\n");
	}
}
// --------------------------------------------------------------------------------------
//			CAN transmit function:
// --------------------------------------------------------------------------------------
void CAN_Transmit(uint8_t data[4], uint8_t msgSelect){
	if(msgSelect==1){
		msgDataPtr[0] = data[0];
		msgDataPtr[1] = data[1];
		msgDataPtr[2] = data[2];
		msgDataPtr[3] = data[3];
		printf("Sending message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2], msgDataPtr[3]); // write colour to UART for debugging
		CANMessageSet(CAN0_BASE, 5, &TxObj[0], MSG_OBJ_TYPE_TX); // send as msg object 1
		printf("Message sent.\n");
	}
	else if(msgSelect==2){
		msgDataPtr2[0] = data[0];
		msgDataPtr2[1] = data[1];
		msgDataPtr2[2] = data[2];
		msgDataPtr2[3] = data[3];
		printf("Sending message 2:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", msgDataPtr2[0], msgDataPtr2[1], msgDataPtr2[2], msgDataPtr2[3]); // write colour to UART for debugging	
		CANMessageSet(CAN0_BASE, 6, &TxObj[1], MSG_OBJ_TYPE_TX); // send as msg object 2
		printf("Message sent.\n");
	}
	else if(msgSelect==3){
		msgDataPtr3[0] = data[0];
		msgDataPtr3[1] = data[1];
		msgDataPtr3[2] = data[2];
		msgDataPtr3[3] = data[3];
		printf("Sending message 2:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", msgDataPtr3[0], msgDataPtr3[1], msgDataPtr3[2], msgDataPtr3[3]); // write colour to UART for debugging	
		CANMessageSet(CAN0_BASE, 7, &TxObj[2], MSG_OBJ_TYPE_TX); // send as msg object 2
		printf("Message sent.\n");
	}
		else if(msgSelect==4){
		msgDataPtr4[0] = data[0];
		msgDataPtr4[1] = data[1];
		msgDataPtr4[2] = data[2];
		msgDataPtr4[3] = data[3];
		printf("Sending message 2:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", msgDataPtr4[0], msgDataPtr4[1], msgDataPtr4[2], msgDataPtr4[3]); // write colour to UART for debugging	
		CANMessageSet(CAN0_BASE, 8, &TxObj[3], MSG_OBJ_TYPE_TX); // send as msg object 2
		printf("Message sent.\n");
	}
	if(errFlag) { // check for errors
		printf("CAN Bus Error\n");
	}
}
// --------------------------------------------------------------------------------------
//			CAN receive functions:
// --------------------------------------------------------------------------------------
void Steady_Receiver(){

	CANMessageSet(CAN0_BASE, 1, &RxObj[0], MSG_OBJ_TYPE_RX);	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 2, &RxObj[1], MSG_OBJ_TYPE_RX);
	CANMessageSet(CAN0_BASE, 3, &RxObj[2], MSG_OBJ_TYPE_RX);	
	CANMessageSet(CAN0_BASE, 4, &RxObj[3], MSG_OBJ_TYPE_RX);	
	
	while (1) {		
		if(rxFlag){		
	
			printf("MsgData[0] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[0][0], MsgData[0][1], MsgData[0][2], MsgData[0][3]);
			printf("MsgData[1] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[1][0], MsgData[1][1], MsgData[1][2], MsgData[1][3]);
			printf("MsgData[2] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[2][0], MsgData[2][1], MsgData[2][2], MsgData[2][3]);
			printf("MsgData[3] received message 1:\tByte 1: %d\tByte 2: %d\tByte 3: %d\t Byte 4: %d\n", MsgData[3][0], MsgData[3][1], MsgData[3][2], MsgData[3][3]);
			rxFlag=0;
		
		if(RxObj[0].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
			printf("CAN message loss detected on 1\n");
			}
		if(RxObj[1].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
			printf("CAN message loss detectedon 2\n");
			}
		if(RxObj[2].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
			printf("CAN message loss detectedon 3\n");
			}
		if(RxObj[3].ui32Flags & MSG_OBJ_DATA_LOST) { // check msg flags for any lost messages
			printf("CAN message loss detectedon 4\n");
			}
		}
	}
}	

void Init_receiver(){
	CANMessageSet(CAN0_BASE, 1, &RxObj[0], MSG_OBJ_TYPE_RX);	// Load msg into CAN peripheral message object 1 so it can trigger interrupts on any matched rx messages
	CANMessageSet(CAN0_BASE, 2, &RxObj[1], MSG_OBJ_TYPE_RX);
	CANMessageSet(CAN0_BASE, 3, &RxObj[2], MSG_OBJ_TYPE_RX);	
	CANMessageSet(CAN0_BASE, 4, &RxObj[3], MSG_OBJ_TYPE_RX);	
}

// --------------------------------------------------------------------------------------
//						struct inits:
// --------------------------------------------------------------------------------------
void Init_Structs(){ // this case has no filter, just always uses identical MsgId's for Tx and Rx
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
	
	TxObj[2].ui32MsgID = 0x03; 
  TxObj[2].ui32MsgIDMask = 0x00;
  TxObj[2].ui32Flags = MSG_OBJ_TX_INT_ENABLE; 
  TxObj[2].ui32MsgLen = sizeof(msgDataPtr3);
  TxObj[2].pui8MsgData = msgDataPtr3;  
	
	TxObj[3].ui32MsgID = 0x04; 
  TxObj[3].ui32MsgIDMask = 0x00;
  TxObj[3].ui32Flags = MSG_OBJ_TX_INT_ENABLE; 
  TxObj[3].ui32MsgLen = sizeof(msgDataPtr4);
  TxObj[3].pui8MsgData = msgDataPtr4;  
	
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
	
	RxObj[2].ui32MsgID = 0x03; // this receive ID acts as the filter 
  RxObj[2].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  RxObj[2].ui32Flags = MSG_OBJ_RX_INT_ENABLE;
  RxObj[2].ui32MsgLen = 8;
	RxObj[2].pui8MsgData = RxMsgData3; // This pointer needs to be set to buffer for Rx Message data. 
	
	RxObj[3].ui32MsgID = 0x04; // this receive ID acts as the filter 
  RxObj[3].ui32MsgIDMask = 0x00; // mask to enable filtration bits
  RxObj[3].ui32Flags = MSG_OBJ_RX_INT_ENABLE;
  RxObj[3].ui32MsgLen = 8;
	RxObj[3].pui8MsgData = RxMsgData4; // This pointer needs to be set to buffer for Rx Message data. 
}