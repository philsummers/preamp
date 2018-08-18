/* 
 * File:   state.h
 * Author: phil
 *
 * Created on 04 February 2018, 14:05
 */

#ifndef STATE_H
#define	STATE_H

#ifdef	__cplusplus
extern "C" {
#endif


enum states {
    S_STANDBY,
    S_STANDBY_2,
    S_POWER_ON,
    S_POWER_ON_2,
    S_IDLE,
    S_MENU_1,
    S_MENU_2,
    S_MENU_3,
    S_MENU_4,
    S_MENU_5,
    S_MENU_6
};



#ifdef	__cplusplus
}
#endif

#endif	/* STATE_H */

