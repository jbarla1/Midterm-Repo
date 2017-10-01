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





void CAN_Master(void);
void CANIntHandler(void);
void CAN_Init(void);
void CAN_Slave(void);