/* 
 * File:   newmain.c
 * Author: phil
 *
 * Created on 07 July 2014, 21:45
 */

#include <stdio.h>
#include <stdlib.h>

#include <p18f4455.h>
//#include <plib/adc.h>
//#include <plib/pconfig.h>
#include <xc.h>

//#include "lcd.h"
#include "LCD-library.h"
#include "volume.h"
#include "switches.h"
#include "state.h"
#include "event_queue.h"
#include "util.h"
#include "stored_config.h"
#include "interface.h"


#pragma config FOSC = HS
#pragma config FCMEN = OFF
#pragma config BORV = 0
#pragma config BOR = 0
#pragma config WDT = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config PBADEN = 0
#pragma config LVP = 0
#pragma config MCLRE = ON
#pragma config CCP2MX = OFF
#pragma config LPT1OSC = OFF
#pragma config DEBUG = OFF
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
//#pragma config WRTC = OFF
//#pragma config WRTB = OFF
//#pragma config WRTD = OFF




#define ENCODER_A PORTBbits.RB4
#define ENCODER_B PORTBbits.RB3
#define ENCODER_DOWN 0
#define ENCODER_UP 1



//volatile char volume;

volatile unsigned char encoder_a;
volatile unsigned char encoder_b;
volatile unsigned char encoder_a_prev;
volatile unsigned char encoder_changed;
volatile unsigned char encoder_dir = ENCODER_DOWN;

char debug_mode = 0;
char bal = 0;

unsigned char current_state = S_STANDBY;

struct config configuration;


void interrupt isr(void) {  

    if (INTCONbits.RBIF) {
        encoder_a = ENCODER_A;
        encoder_b = ENCODER_B;
        
        if ((!encoder_a) && (encoder_a_prev)) {
            if (encoder_b) {
                encoder_changed = 1;
                encoder_dir = ENCODER_UP;
            } else {
                encoder_changed = 1;
                encoder_dir = ENCODER_DOWN;
            }
        }
        encoder_a_prev = encoder_a;        
        PORTB = PORTB;
        INTCONbits.RBIF = 0;    // Clear the flag
    } else if (INTCONbits.T0IF) {

        // Clear the flag
        INTCONbits.T0IF = 0;
    }
}

void init_timer() {
    // Set timer0 to 16 bits
    T0CONbits.T08BIT = 0;

    // Internal cycle clock
    T0CONbits.T0CS = 0;

    // Prescaler
    T0CONbits.PSA = 0;

    // Prescaler to 1:32
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS0 = 0;

    // Enable the timer
    T0CONbits.TMR0ON = 1;

    // Enable timer0 interrupt
    INTCONbits.TMR0IE = 1;
}

void Wait(unsigned int delay)
{
   for(;delay;delay--)
      __delay_us(100);
}

void delay_ms(unsigned short int i) {
    for (unsigned char x=0; x<=20; x++) {
        __delay_ms(10);
    }
}

void delay_s(unsigned char i) {
    for (int i=0; i<=100; i++) {
        __delay_ms(10);
    }
}

// Perform the startup sequence
void startup_sequence() {
    clear_all_sources();
    for (char i=0; i<8; i++) {
        set_switch_led(i);
        update_switches_and_relays();
        delay_ms(400);
        clear_switch_led(i);
        update_switches_and_relays();
    }
    set_mute_led();
    delay_ms(400);
    clear_mute_led();
    set_view_led();
    delay_ms(400);
    clear_view_led();
    set_store_led();
    delay_ms(400);
    clear_store_led();
    delay_ms(3000);
    for (char i=0; i<8; i++) {
        set_switch_led(i);
        update_switches_and_relays();
    }
    set_mute_led();
    set_view_led();
    set_store_led();
    delay_s(3);
    clear_all_switch_leds();
    update_switches_and_relays();
    clear_mute_led();
    clear_view_led();
    clear_store_led();
    LCD_home();
    LCD_setCursor(1,1);
    LCD_displayString("  HiFi  Preamp  ");
    LCD_setCursor(1,2);
    LCD_displayString("      v0.1      ");
    delay_s(5);
    LCD_clear();
}

void init_lcd_chars(void) {
    // Char 0 - Left arrow
    LCD_code(0b0001000000);
    LCD_code(0b1000000010);

    LCD_code(0b0001000001);
    LCD_code(0b1000000110);

    LCD_code(0b0001000010);
    LCD_code(0b1000001110);

    LCD_code(0b0001000011);
    LCD_code(0b1000011110);

    LCD_code(0b0001000100);
    LCD_code(0b1000001110);

    LCD_code(0b0001000101);
    LCD_code(0b1000000110);

    LCD_code(0b0001000110);
    LCD_code(0b1000000010);
    
    LCD_code(0b0001000111);
    LCD_code(0b1000000000);

    // Char 1 - Right Arrow
    LCD_code(0b0001001000);
    LCD_code(0b1000001000);

    LCD_code(0b0001001001);
    LCD_code(0b1000001100);

    LCD_code(0b0001001010);
    LCD_code(0b1000001110);

    LCD_code(0b0001001011);
    LCD_code(0b1000001111);

    LCD_code(0b0001001100);
    LCD_code(0b1000001110);

    LCD_code(0b0001001101);
    LCD_code(0b1000001100);

    LCD_code(0b0001001110);
    LCD_code(0b1000001000);

    LCD_code(0b0001001111);
    LCD_code(0b1000000000);

    // Char 2 - Double vert bars
    LCD_code(0b0001010000);
    LCD_code(0b1000011011);

    LCD_code(0b0001010001);
    LCD_code(0b1000011011);

    LCD_code(0b0001010010);
    LCD_code(0b1000011011);

    LCD_code(0b0001010011);
    LCD_code(0b1000011011);

    LCD_code(0b0001010100);
    LCD_code(0b1000011011);

    LCD_code(0b0001010101);
    LCD_code(0b1000011011);

    LCD_code(0b0001010110);
    LCD_code(0b1000011011);

    LCD_code(0b0001010111);
    LCD_code(0b1000000000);

}

int main(int argc, char ** argv) {
    char volstring[8];
    char switch_pressed = 0;    

    encoder_changed = 0;
    volume = 0;
    
    // Init port B
    PORTB = 0x00;
    LATB = 0x00;
    TRISB = 0x00;

    // Initialise the global config state
    configuration.source = 0;
    configuration.volume[0] = 128;
    configuration.volume[1] = 128;
    configuration.volume[2] = 128;
    configuration.volume[3] = 128;
    configuration.volume[4] = 128;
    configuration.volume[5] = 128;
    configuration.volume[6] = 128;
    configuration.balance[0] = 5;
    configuration.balance[1] = 5;
    configuration.balance[2] = 5;
    configuration.balance[3] = 5;
    configuration.balance[4] = 5;
    configuration.balance[5] = 5;
    configuration.balance[6] = 5;

    init_switches();
    init_shiftreg();
    init_volume();
    init_timer();
       
    INTCONbits.GIE = 1;   // Enable interrupts globally


   //Initialize the LCD Module
   LCD_init();
   init_lcd_chars();

   LCD_off();

   //Clear the Module
   LCD_clear();
   LCD_home();
   LCD_cursorOff();

   LCD_on();

   //Let the Module start up
    Wait(100);

   // Turn off the backlight
   backlight_off();
  /* LCD_setCursor(1,1);
   LCD_displayChar(0b00000000);
   LCD_setCursor(2,1);
   LCD_displayChar(0b00000001);
   LCD_setCursor(3,1);
   LCD_displayChar(0b00000010);

   while(1) {
       asm("nop");
   }
*/

   if (read_switches() == SWITCH_STORE)
       debug_mode = 1;
        
   Wait(1000);   

   if (debug_mode) {
       backlight_on();
       LCD_setCursor(1,2);
       LCD_displayString("***DEBUG MODE***");
   }

   // Main loop
   while (1) {
       switch (current_state) {
           case S_STANDBY:
               LCD_clear();
               backlight_off();
               set_standby_led();
               clear_mute_led();
               clear_view_led();
               clear_store_led();
               clear_all_switch_leds();
               clear_all_sources();
               update_switches_and_relays();
               volume = 0;
               set_volume(volume);
               current_state = S_STANDBY_2;
               break;
           case S_STANDBY_2:
               __delay_us(100);
               if (read_switches_synchronous() == SWITCH_POWER) {
                   current_state = S_POWER_ON;
                   break;
               }
               break;
           case S_POWER_ON:
               clear_standby_led();
               backlight_on();
               //startup_sequence();
               set_led_and_source(DEFAULT_SOURCE);
               update_switches_and_relays();
               volume = DEFAULT_VOLUME;
               set_volume(volume);
               volumeDB(volstring);
               LCD_setCursor(10,2);
               LCD_displayString(volstring);
               current_state = S_IDLE;
               break;

           case S_IDLE:
               __delay_us(100);
               switch_pressed = read_switches_synchronous();

               switch (switch_pressed) {
                   case SWITCH_POWER:
                       current_state = S_STANDBY;
                       break;
                   case SWITCH_SOURCE_1:
                       set_led_and_source(0);
                       configuration.source = 0;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_2:
                       set_led_and_source(1);
                       configuration.source = 1;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_3:
                       set_led_and_source(2);
                       configuration.source = 2;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_4:
                       set_led_and_source(3);
                       configuration.source = 3;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_5:
                       set_led_and_source(4);
                       configuration.source = 4;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_6:
                       set_led_and_source(5);
                       configuration.source = 5;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_SOURCE_7:
                       set_led_and_source(6);
                       configuration.source = 6;
                       toggle_mute();
                       update_switches_and_relays();
                       toggle_mute();
                       break;
                   case SWITCH_MUTE:
                       toggle_mute();
                       volumeDB(volstring);
                       LCD_setCursor(10,2);
                       LCD_displayString(volstring);
                       break;
                   case SWITCH_VIEW:
                       current_state = S_MENU_1;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }

               if (encoder_changed) {
                   if (encoder_dir == ENCODER_UP) {
                       if (volume < 255) {
                           volume++;
                           configuration.volume[configuration.source]++;
                       }
                   } else {
                       if (volume > 0) {
                           volume--;
                           configuration.volume[configuration.source]--;
                       }
                   }
                   set_volume(configuration.volume[configuration.source]);
                   encoder_changed = 0;
                   volumeDB(volstring);
                   LCD_setCursor(10,2);
                   LCD_displayString(volstring);
               }
               break;

           case S_MENU_1:   
               LCD_setCursor(1,1);
               LCD_displayString(STRING_BALANCE);
               LCD_slider(bal);
               if (encoder_changed) {
                   if (encoder_dir == ENCODER_UP) {
                       if (bal < 13) {
                           bal++;
                       }
                   } else {
                       if (bal > 0) {
                           bal--;
                       }
                   }
                   encoder_changed = 0;
               }
               switch_pressed = read_switches_synchronous();
               switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_MENU_2;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

           case S_MENU_2:
               LCD_clear();
               LCD_setCursor(1,1);
               LCD_displayString(STRING_CENTRE);
               switch_pressed = read_switches_synchronous();
               switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_MENU_3;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

           case S_MENU_3:
               LCD_clear();
               LCD_setCursor(1,1);
               LCD_displayString(STRING_REAR);
               switch_pressed = read_switches_synchronous();
                switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_MENU_4;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

           case S_MENU_4:
               LCD_clear();
               LCD_setCursor(1,1);
               LCD_displayString(STRING_REAR_BAL);
               switch_pressed = read_switches_synchronous();
                switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_MENU_5;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

           case S_MENU_5:
               LCD_clear();
               LCD_setCursor(1,1);
               LCD_displayString(STRING_LFE);
               switch_pressed = read_switches_synchronous();
                switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_MENU_6;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

            case S_MENU_6:
                LCD_clear();
               LCD_setCursor(1,1);
               LCD_displayString(STRING_SETUP);
               switch_pressed = read_switches_synchronous();
                switch (switch_pressed) {
                   case SWITCH_VIEW:
                       current_state = S_IDLE;
                       LCD_clear();
                       break;
                   case SWITCH_STORE:
                       break;
               }
               break;

           default:
               LCD_setCursor(1,1);
               LCD_displayString("INVALID STATE");

       }
   }

   //set_standby_led();
     
   Wait(100);
   
   
    return (EXIT_SUCCESS);
}

