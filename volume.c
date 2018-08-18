/********************************************
 * PGA2310PA Volume control module          *
 *                                          *
 ********************************************/

#include <p18f4455.h>
#include <stdio.h>
#include "switches.h"

volatile char volume;
char muted_volume;
char is_muted = 0;

/* Connections */

/* Port E
     Pin RE0 - Master volume Clock
     Pin RE1 - Master volume Data
     Pin RE2 - Master volume Chipselect / latch
     Pin RB3 - Volume control Encoder input B
     Pin RB4 - Volume control Encoder input A

 */



void init_volume(void) {

    // Clear the port bits
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
    PORTEbits.RE2 = 0;

    // Clear the latches as well
    LATEbits.LE0 = 0;
    LATEbits.LE1 = 0;
    LATEbits.LE2 = 0;

    // Turn off the AtoD on all but the lowest bit
    ADCON1 = 0x0E;  // Disable a2d on all bar AN0
    ADCON0 = 0x01;
    ADCON2 = 0xA5;

    // Disable comparators
    CMCON = 0x07;

    // Set TRISE to outputs (only bits 1,2,3 are implemented)
    TRISE = 0x00;

    TRISBbits.RB3 = 1;    // Make RB3 an input
    TRISBbits.RB4 = 1;    // Make RB4 an input

    INTCON2bits.RBIP = 1; // Make the port change interrupt high priority
    INTCONbits.RBIF = 0;  // Clear the port change interrupt flag bit
    INTCONbits.RBIE = 1;  // Set the port change interrupt enable bit
}

void set_volume(unsigned char vol) {
    unsigned char vbitsel = 0x80;

   // Bring chip select low
   LATEbits.LE2 = 0;

   // Output the volume
   for (char i = 0; i< 16; i++) {
       // Clear the clock
       LATEbits.LE0 = 0;

       // Set the data line
       if (vol & vbitsel) {
           LATEbits.LE1 = 1;
       } else {
           LATEbits.LE1 = 0;
       }
       
       // Clock the data in
       LATEbits.LE0 = 1;

       // Shift the bit in bitselect one to the right
       vbitsel = vbitsel >> 1;
       if (vbitsel == 0) {
           vbitsel = 0x80;
       }
   }

   // Raise the chipselect to latch the data
   LATEbits.LE2 = 1;
}

void toggle_mute() {
    if (is_muted) {
        volume = muted_volume;
        set_volume(volume);
        clear_mute_led();
        is_muted = 0;
    } else {
        muted_volume = volume;
        volume = 0;
        set_volume(0);
        set_mute_led();
        is_muted = 1;

    }
}

void volumeDB(char *o) {
    int modulus = 0;
    int gain = 0;
    int gainDec = 0;

    if (volume == 0) {
        o[0] = ' ';
        o[1] = ' ';
        o[2] = ' ';
        o[3] = 'M';
        o[4] = 'u';
        o[5] = 't';
        o[6] = 'e';
        o[7] = '\0';
        return;
    }


    if (volume == 192) {
        gain = 0;
        gainDec = 0;
        o[0] = ' ';
    }
    else if (volume > 192) {
        gain = 31 - ((255-volume)/2);
        modulus = (254-volume) %2;
        o[0]='+';
    }
    else if (volume < 192) {
        gain = ((254-volume)/2)-31;
        modulus = (254-volume)%2;
        o[0]='-';
    }
    if (modulus)
        gainDec = 5;
    else
        gainDec = 0;

    o[1] = '0'+(gain / 10) % 10;
    o[2] = '0'+gain %10;
    o[3] = '.';
    o[4] = '0'+gainDec % 10;
    o[5] = 'd';
    o[6] = 'B';
    o[7] = '\0';
}
