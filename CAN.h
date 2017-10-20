#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "project.h"

#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "stdbool.h"
#include "driverlib/can.h"
#include "inc/hw_memmap.h"
#include "./driverlib/sysctl.h"
	
//struct msg {
//	uint32_t ui32MsgID;
//	uint32_t ui32MsgIDMask;
	//uint32_t ui32Flags;
//	uint32_t ui32MsgLen;
//	uint8_t  *pui8MsgData;
//};

//struct msg msg1, msg2, msg3, msg4;



// create function to initialize every struct value 



void Init_Receiver(void);
void CAN_Transmit(uint8_t data[4]);
void CANIntHandler(void);
void CAN_Init(void);
void CAN_Slave(void);
void Init_Structs(void);