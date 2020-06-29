/* 
 * File:   timer.h
 * Author: sladekm
 *
 * Created on February 29, 2020, 5:32 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "system.h"


#ifdef	__cplusplus
extern "C" {
#endif

void initTimer4_10ms();
void initTimer3_100ms();
void initTimer2_250ms();
void disableTimer2();

#define _call10msEventHandler() LED1_handler()
#define _call100msEventHandler() LED2_handler()
#define _call250msEventHandler() change_scan_state_flag()


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

