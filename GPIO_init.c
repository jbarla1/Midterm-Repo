#include "GPIO_init.h"


void GPIO_Setup(void) {

 volatile uint32_t ui32Loop;
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
 GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
