/*
 * drv_hc_sr04.h
 *
 *  Created on: 2025年5月28日
 *      Author: DAMIAN_CHEN
 */

#ifndef DRV_HC_SR04_H_
#define DRV_HC_SR04_H_


#include "hal_data.h"
#include "drv_timer.h"


typedef enum{
    ECHO_BUSY=1,
    ECHO_IDLE=0,
}echo_status_e;


extern float hc_sr04_length;


void hc_sr04_init(void);

void hc_sr04_trig(void);

void hc_sr04_echo(void);


#endif /* DRV_HC_SR04_H_ */
