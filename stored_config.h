/* 
 * File:   stored_config.h
 * Author: phil
 *
 * Created on 02 March 2018, 11:31
 */

#ifndef STORED_CONFIG_H
#define	STORED_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DEFAULT_VOLUME 128
#define DEFAULT_SOURCE 0
#define NUM_SOURCES 7

struct config {
    char volume[NUM_SOURCES];
    char balance[NUM_SOURCES];
    char source;
    char mute;
};


#ifdef	__cplusplus
}
#endif

#endif	/* STORED_CONFIG_H */

