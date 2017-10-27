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


#define NUM_RX_MESSAGES 4
#define NUM_TX_MESSAGES 4

extern unsigned char MsgData[NUM_RX_MESSAGES][4];

void Init_receiver(void);
void Steady_Receiver(void);
void CAN_Transmit(uint8_t data[4], uint8_t msgSelect);
void CANIntHandler(void);
void CAN_Init(void);
void CAN_Slave(void);
void Init_Structs(void);
void Init_Structs1(void);
void Init_Structs2(void);
void Init_Structs3(void);