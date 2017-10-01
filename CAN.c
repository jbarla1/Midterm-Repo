#include "CAN.h"
#include <stdio.h>
#include <stdint.h>

unsigned int sysClock; // clockspeed in hz
volatile bool errFlag = 0; // transmission error flag

void CAN_Init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	GPIOPinConfigure(GPIO_PE5_CAN0TX);
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	CANInit(CAN0_BASE);
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
	CANIntRegister(CAN0_BASE, CANIntHandler); // use dynamic vector table allocation
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	CANEnable(CAN0_BASE);
}

void CANIntHandler(void) {
	
	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status

	if(status == CAN_INT_INTID_STATUS) { // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		errFlag = 1;
	} else if(status == 1) { // message object 1
		CANIntClear(CAN0_BASE, 1); // clear interrupt
		errFlag = 0; // clear any error flags
	} else { // should never happen
	printf("Unexpected CAN bus interrupt\n");
	}
}


void CAN_Set(void) {
	tCANMsgObject msg; // the CAN message object
	unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
	unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes
	
	
	msgData = 0;
	msg.ui32MsgID = 1;
	msg.ui32MsgIDMask = 0;
	msg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
	msg.ui32MsgLen = sizeof(msgDataPtr);
	msg.pui8MsgData = msgDataPtr;

	uint32_t t = 0; // loop counter
	float freq = 0.1; // frequency scaler
	int choice =0;
	uint32_t x = 20;
	while(1) {

	//	printf("Red, blue, or green? Enter 1, 2, or 3 respectively.");
	//	char choice = getchar();
		choice++;
		// set up next colour (scale sinf (0-1) to 0-255)
		if(choice==4){
			choice = 1;
		}
		
		switch (choice) {
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
		x++;
		if(x == 100){
			x = 20;
		}
		
		delayMS(x); // wait 100ms
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xF);
		delayMS(x);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
		
		if(errFlag) { // check for errors
		printf("CAN Bus Error\n");
		}

		t++; // overflow is fine
	}
	
}


