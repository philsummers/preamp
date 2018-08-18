/********************************************************************

16X2 ALPHANEUMERIC LCD INTERFACING LIBRARY FOR PIC 18F MCUS
-----------------------------------------------------------

Easy to use library for interfacing 16x2 lcd in 4 bit mode.
MCU: PIC18FXXXX Series from Microchip.
Compiler: HI-TECH C Compiler for PIC18 MCUs (http://www.htsoft.com/)

Copyrights 2008-2009 Avinash Gupta
eXtreme Electronics, India

For More Info visit
http://www.eXtremeElectronics.co.in

Mail: me@avinashgupta.com

********************************************************************/

#include <htc.h>

#include "lcd.h"


#define LCD_DATA_LAT 	LAT(LCD_DATA)
#define LCD_E_LAT 		LAT(LCD_E)
#define LCD_RS_LAT 		LAT(LCD_RS)
#define LCD_RW_LAT 		LAT(LCD_RW)

#define LCD_DATA_TRIS 	TRIS(LCD_DATA)
#define LCD_E_TRIS 		TRIS(LCD_E)
#define LCD_RS_TRIS 	TRIS(LCD_RS)
#define LCD_RW_TRIS 	TRIS(LCD_RW)

#define LCD_DATA_PORT	PORT(LCD_DATA)

#define SET_E() (LCD_E_LAT|=(1<<LCD_E_POS))
#define SET_RS() (LCD_RS_LAT|=(1<<LCD_RS_POS))
#define SET_RW() (LCD_RW_LAT|=(1<<LCD_RW_POS))

#define CLEAR_E() (LCD_E_LAT&=(~(1<<LCD_E_POS)))
#define CLEAR_RS() (LCD_RS_LAT&=(~(1<<LCD_RS_POS)))
#define CLEAR_RW() (LCD_RW_LAT&=(~(1<<LCD_RW_POS)))




void LCDByte(uint8_t c,uint8_t isdata)
{
//Sends a byte to the LCD in 4bit mode
//cmd=0 for data
//cmd=1 for command


//NOTE: THIS FUNCTION RETURS ONLY WHEN LCD HAS PROCESSED THE COMMAND

uint8_t hn,ln;			//Nibbles
uint8_t temp;

//hn=c>>4;
//ln=(c & 0x0F);
hn = (c & 0xF0);
ln = c<< 4;


if(isdata==0)
	CLEAR_RS();
else
	SET_RS();

__delay_us(1);		//tAS

SET_E();
__delay_us(2);

//Send high nibble

temp=(LCD_DATA_LAT & 0X0F)|(hn);
LCD_DATA_LAT=temp;

__delay_us(2);			//tEH

//Now data lines are stable pull E low for transmission

CLEAR_E();

__delay_us(2);

//Send the lower nibble
SET_E();

temp=(LCD_DATA_LAT & 0X0F)|(ln);

LCD_DATA_LAT=temp;

__delay_us(2);			//tEH

//SEND

CLEAR_E();

__delay_us(2);			//tEL

if (isdata == 0)
    __delay_us(200);
else
    __delay_us(5000);

//LCDBusyLoop();
}

/*void LCDBusyLoop()
{
	//This function waits till lcd is BUSY

	uint8_t busy,status=0x00,temp;

	//Change Port to input type because we are reading data
	LCD_DATA_TRIS|=0xF0;

	//change LCD mode
	SET_RW();		//Read mode
	CLEAR_RS();		//Read status

	//Let the RW/RS lines stabilize

	__delay_us(1);		//tAS

	
	do
	{

		SET_E();

		//Wait tDA for data to become available
		__delay_us(2);

		status=LCD_DATA_PORT;
		//status=status<<4;

		__delay_us(2);

		//Pull E low
		CLEAR_E();
		__delay_us(2);	//tEL

		SET_E();
		__delay_us(2);

		temp=LCD_DATA_PORT;
		temp&=0xF0;
                temp >>4;

		status=status|temp;

		busy=status & 0b10000000;

		__delay_us(2);
		CLEAR_E();
		__delay_us(2);	//tEL
	}while(busy);

	CLEAR_RW();		//write mode
	//Change Port to output
	LCD_DATA_TRIS&=0x0F;

}
*/

void LCDInit(uint8_t style)
{
	/*****************************************************************

	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK :The cursor is blinking type
	LS_ULINE :Cursor is "underline" type else "block" type

	*****************************************************************/

	//After power on Wait for LCD to Initialize
	__delay_ms(30);        // Was 30ms
        __delay_ms(30);
        __delay_ms(30);
        __delay_ms(10);

	//Set IO Ports
	LCD_DATA_TRIS&=(0x0F);
	LCD_E_TRIS&=(~(1<<LCD_E_POS));
	LCD_RS_TRIS&=(~(1<<LCD_RS_POS));
	LCD_RW_TRIS&=(~(1<<LCD_RW_POS));

        CLEAR_RW();
	CLEAR_RS();

        SET_E();

	LCD_DATA_LAT=0b00110000;
        __delay_us(1);
	CLEAR_E();
        __delay_ms(5);

        SET_E();
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);

        SET_E();
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);

        // Now send Initial Function Set
        SET_E();
        LCD_DATA_LAT=0b00100000;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);

        LCDCmd(0b00101000);

        /*SET_E();
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        SET_E();
        LCD_DATA_LAT=0b10000000;  // N & F
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
*/

        // Display ON/OFF
        /*SET_E();
        LCD_DATA_LAT=0x00;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        SET_E();
        LCD_DATA_LAT=0b1000000;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        */
        LCDCmd(0b00001000);


        // Clear Display
        /*SET_E();
        LCD_DATA_LAT=0x00;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        SET_E();
        LCD_DATA_LAT=0b00010000;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(5);
*/
        LCDCmd(0b00000001);

        // Entry Mode Set
        /*SET_E();
        LCD_DATA_LAT=0x00;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        SET_E();
        LCD_DATA_LAT=0b01000000;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
*/
        LCDCmd(0b00000100);
        // Init ends

        // Turn Display On
        // Entry Mode Set
        /*SET_E();
        LCD_DATA_LAT=0x00;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
        SET_E();
        LCD_DATA_LAT=0b11000000;
        __delay_us(1);
        CLEAR_E();
        __delay_ms(1);
*/
        LCDCmd(00001100);


	//LCDCmd(0b00101000);			//function set 4-bit,2 line 5x7 dot format
}

void LCDInit_old(uint8_t style)
{
	/*****************************************************************
	
	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK :The cursor is blinking type
	LS_ULINE :Cursor is "underline" type else "block" type

	*****************************************************************/
	
	//After power on Wait for LCD to Initialize
	__delay_ms(100);        // Was 30ms
	
	//Set IO Ports
	LCD_DATA_TRIS&=(0x0F);
	LCD_E_TRIS&=(~(1<<LCD_E_POS));
	LCD_RS_TRIS&=(~(1<<LCD_RS_POS));
	LCD_RW_TRIS&=(~(1<<LCD_RW_POS));


	LCD_DATA_LAT&=0X0F;
	CLEAR_E();
	CLEAR_RW();
	CLEAR_RS();

	//Set 4-bit mode
	__delay_us(1);	//tAS

	SET_E();
	LCD_DATA_LAT|=(0b00100000); //[B] To transfer 0b00100000 i was using LCD_DATA_PORT|=0b00100000
	__delay_us(1);
	CLEAR_E();
	__delay_us(1);
	
	//Wait for LCD to execute the Functionset Command
	//LCDBusyLoop();                                    //[B] Forgot this delay
        __delay_us(15000);

	//Now the LCD is in 4-bit mode

	LCDCmd(0b00001000|style);	//Display On
	LCDCmd(0b00101000);			//function set 4-bit,2 line 5x7 dot format
}
void LCDWriteString(const char *msg)
{
	/*****************************************************************
	
	This function Writes a given string to lcd at the current cursor
	location.

	Arguments:
	msg: a null terminated string to print


	*****************************************************************/
 while(*msg!='\0')
 {
	LCDData(*msg);
	msg++;
 }
}

void LCDWriteInt(int val,unsigned int field_length)
{
	/***************************************************************
	This function writes a integer type value to LCD module

	Arguments:
	1)int val	: Value to print

	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val

	****************************************************************/

	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) LCDData('-');
	for(i=j;i<5;i++)
	{
	LCDData(48+str[i]);
	}
}
void LCDGotoXY(uint8_t x,uint8_t y)
{
 if(x<40)
 {
  if(y) x|=0b01000000;
  x|=0b10000000;
  LCDCmd(x);
  }
}





