#include "Includes.h"


// Function definitions 

void TwoSecDelay(void)
{
	for(int i=0;i<20;i++)
       __delay_ms(100);
}


void InitIOPorts(void)
{
	// Configure Unused pins as Inputs
	TRISA |= 0x3F;
	TRISC  = 0xFF;
	TRISD |= 0xF8;
	TRISE |= 0x07;
}