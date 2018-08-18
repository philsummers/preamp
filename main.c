/*  Name     : main.c
 *  Purpose  : Main file for Using LCD.
 *  Author   : M.Saeed Yasin
 *  Date     : 08-07-11
 *  Revision : None
 */

#include "Includes.h"


// Configuration Word for PIC16F877 (For old version of HiTech C)
//__CONFIG(HS & WDTDIS & PWRTEN & BOREN & LVPEN & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

// Configuration Word for PIC16F877 (For new version of HiTech C)
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_ON & DEBUG_OFF);


void main(void)
{
	InitIOPorts();    // Initialize IO ports of the microcontroller
	InitLCD();        // Initialize the LCD

	WriteStringToLCD("ding dong ding dong");

	while(1)
	{
	//	ScrollLCD(ScrollScreen, ToRight, 1);

		__delay_ms(100);
		__delay_ms(100);
	}
}