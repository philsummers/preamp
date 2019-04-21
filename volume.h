/* 
 * File:   volume.h
 * Author: phil
 *
 * Created on 17 December 2017, 21:59
 */

#ifndef VOLUME_H
#define	VOLUME_H

extern volatile char volume_left;
extern volatile char volume_right;

void init_volume(void);
void set_volume(unsigned char vol_left, unsigned char vol_right);
void calculate_output_volumes(unsigned char vol,
                              unsigned char bal,
                              unsigned char *left,
                              unsigned char *right);
void volumeDB(unsigned char volume, char *o);
void toggle_mute();

#endif	/* VOLUME_H */

