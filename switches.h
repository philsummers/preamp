/* 
 * File:   switches.h
 * Author: phil
 *
 * Created on 20 December 2017, 11:59
 */

#ifndef SWITCHES_H
#define	SWITCHES_H

#define SWITCH_NONE 0
#define SWITCH_POWER 1
#define SWITCH_SOURCE_1 2
#define SWITCH_SOURCE_2 3
#define SWITCH_SOURCE_3 4
#define SWITCH_SOURCE_4 5
#define SWITCH_SOURCE_5 6
#define SWITCH_SOURCE_6 7
#define SWITCH_SOURCE_7 8
#define SWITCH_MUTE 9
#define SWITCH_VIEW 10
#define SWITCH_STORE 11

void init_shiftreg(void);
void init_switches(void);
char read_switches(void);
char read_switches_synchronous(void);

void set_standby_led(void);
void clear_standby_led(void);
void set_mute_led(void);
void clear_mute_led(void);
void set_view_led(void);
void clear_view_led(void);
void set_store_led(void);
void clear_store_led(void);
void set_switch_led(char s);
void clear_switch_led(char s);
void clear_all_switch_leds();
void set_source(char s);
void clear_source(char s);
void clear_all_sources();
void update_switches_and_relays();
void set_led_and_source(char s);



#endif	/* SWITCHES_H */

