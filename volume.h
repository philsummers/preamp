/* 
 * File:   volume.h
 * Author: phil
 *
 * Created on 17 December 2017, 21:59
 */

#ifndef VOLUME_H
#define	VOLUME_H

extern volatile char volume;

void init_volume(void);
void set_volume(unsigned char vol);
void volumeDB(char *o);
void toggle_mute();

#endif	/* VOLUME_H */

