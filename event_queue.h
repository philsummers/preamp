/* 
 * File:   event_queue.h
 * Author: phil
 *
 * Created on 10 February 2018, 14:36
 */

#ifndef EVENT_QUEUE_H
#define	EVENT_QUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define EVT_VOL_UP      0x01
#define EVT_VOL_DOWN    0x02
#define EVT_MUTE        0x03
#define EVT_VIEW       0x04
#define EVT_STORE        0x05
#define EVT_PWR         0x06
#define EVT_BUT_1       0x07
#define EVT_BUT_2       0x08
#define EVT_BUT_3       0x09
#define EVT_BUT_4       0x0a
#define EVT_BUT_5       0x0b
#define EVT_BUT_6       0x0c
#define EVT_BUT_7       0x0d

char enqueue_event(char evt);
char dequeue_event();
char events_in_queue();


#ifdef	__cplusplus
}
#endif

#endif	/* EVENT_QUEUE_H */

