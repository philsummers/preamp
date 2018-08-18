/* 
 * File:   util.h
 * Author: phil
 *
 * Created on 01 March 2018, 22:08
 */

#ifndef UTIL_H
#define	UTIL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define bit_test(val,bit) (((val) & (1 << (bit))) >> (bit))
#define bit_set(val,bit) (val|=(1<<bit))
#define bit_clear(val,bit) (val&=~(1<<bit))



#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

