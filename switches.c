/*********************************
 * Preamp switches               *
 * ******************************/

/* Connections */

/* Standby LED: RA5
   Store LED:   RA3
   View  LED:   RA2
   Mute  LED:   RA1
   Switch Analogue:  RA0
   Switch Latch:  RC2
   Switch Clock: RC1
   Switch Data: RC0

 */

#include <p18f4455.h>
#include "switches.h"
#include "util.h"
#include "LCD-library.h"

unsigned int relay_source = 0;
unsigned int led = 0;


void init_shiftreg(void) {

    // Disable USB for port C availability
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1;

    // Disable timer 1 and 3 for port C
    T1CONbits.TMR1ON = 0;
    T3CONbits.TMR3ON = 0;

    // Disable CCP1 and CCP2 for port C
    CCP1CON = 0x00;
    CCP2CON = 0x00;

    // Set the port C direction and clear
    PORTC = 0x00;
    LATC = 0x00;
    TRISC = 0x00;

}

void init_switches(void) {
    
    // Setup the direction reg for port A LEDs
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA5 = 0;

    // Turn off the right switches
    LATAbits.LATA1 = 1;
    LATAbits.LATA2 = 1;
    LATAbits.LATA3 = 1;
}

void set_standby_led(void) {
    LATAbits.LATA5 = 1;
}

void clear_standby_led(void) {
    LATAbits.LATA5 = 0;
}

void set_mute_led(void) {
    LATAbits.LATA1 = 1;
}

void clear_mute_led(void) {
    LATAbits.LATA1 = 0;
}

void set_view_led(void) {
    LATAbits.LATA2 = 1;
}

void clear_view_led(void) {
    LATAbits.LATA2 = 0;
}

void set_store_led(void) {
    LATAbits.LATA3 = 1;
}

void clear_store_led(void) {
    LATAbits.LATA3 = 0;
}


/*
  Perform an A2D conversion to find out if any of the switches have
  been pressed
 
   Return value is either 0 for no switches or the number of the switch
   that has been pressed
 */
char read_switches(void) {

    int a2dval;

   // Values:
   // 57 - Threshold 89
   // 121 - Threshold 154
   // 188 - Threshold 222
   // 257 - Threshold 286
   // 315 - Threshold 345
   // 376 - Threshold 406
   // 437 - Threshold 467
   // 497 - Threshold 525
   // 554 - Threshold 585
   // 617 - Threshold 645
   // 674 - Threshold 704

    // Initiate an A2D conversion
     ADCON0bits.GO_DONE = 1;

     // Hard loop currently
     //   This should really be interrupt based
     while (ADCON0bits.GO_DONE) {
           asm("NOP");
     }
      a2dval = ((ADRESH << 8)+ADRESL);
     
       if (a2dval < 89) {
           return SWITCH_POWER;
       } else if (a2dval < 154 ) {
           return SWITCH_SOURCE_1;
       } else if (a2dval < 222) {
           return SWITCH_SOURCE_2;
       } else if (a2dval < 286) {
           return SWITCH_SOURCE_3;
       } else if (a2dval < 345) {
           return SWITCH_SOURCE_4;
       } else if (a2dval < 406) {
           return SWITCH_SOURCE_5;
       } else if (a2dval < 467) {
           return SWITCH_SOURCE_6;
       } else if (a2dval < 525) {
           return SWITCH_SOURCE_7;
       } else if (a2dval < 585) {
           return SWITCH_MUTE;
       } else if (a2dval < 645) {
           return SWITCH_VIEW;
       } else if (a2dval < 704) {
           return SWITCH_STORE;
       }
      return SWITCH_NONE;

}

// Reads the switches and either
//   Returns immediately if none pressed
//   Waits for the switch to be released if one was pressed
//
//  Returns:  SWITCH_NONE if none pressed
//            SWITCH_XXXX if was pressed
char read_switches_synchronous() {
    char s;

    s = read_switches();

    if (s == SWITCH_NONE)
        return SWITCH_NONE;

     while (read_switches() != SWITCH_NONE) {
        __delay_us(100);
     }
    return s;
}

void set_switch_led(char s) {
    // Set an individual led
    bit_set(led,s);
}

void clear_switch_led(char s) {
    // Clear an individual led
    bit_clear(led,s);
}

void clear_all_switch_leds() {
    led = 0;
}

void set_source(char s) {
    // Set  the source
    bit_set(relay_source,s+1);
}

void clear_source(char s) {
    // Clear the source
    bit_clear(relay_source,s+1);
}

void clear_all_sources() {
    relay_source = 0;
}

void set_led_and_source(char s) {
    led = 0;
    relay_source=0;
    bit_set(led,s+1);
    bit_set(relay_source,s+2);
}

void update_switches_and_relays(void) {
     // Clear the latch bit
     LATCbits.LC2 = 0;

    // LCD_clear();
    // LCD_home();

     // LEDs
     for (char i = 8; i != 0; i--) {
       // Set the data bit
       LATCbits.LC0 = bit_test(led,i);      

       // Waggle the clock
       LATCbits.LC1 = 1;
       asm("NOP");
       LATCbits.LC1 = 0;

     //  LCD_displayNumber(bit_test(led,i));
     //  LCD_moveRight(1);
     }

     // Relays
     for (char i = 8; i!= 0; i--) {
       LATCbits.LC0 = bit_test(relay_source,i);
       LATCbits.LC1 = 1;
       asm("NOP");
       LATCbits.LC1 = 0;

     //  LCD_displayNumber(bit_test(relay_source,i));
     //  LCD_moveRight(1);

     }

     // And latch the data
     LATCbits.LC2 = 1;

}
