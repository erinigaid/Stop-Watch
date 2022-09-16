/*
 * main.h
 *
 *  Created on: Sep 15, 2022
 *      Author: Erini
 */

/*Head file guard*/
#ifndef MAIN_H_
#define MAIN_H_

/*Linking Section*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "STD_TYPES.h"

/*Macros*/
#define HRS  0
#define MINS 1
#define SECS 2


/*Function Prototype*/
void GPIO(void);

void Timer1_CTC_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);

void CountUp(void);
void DisplayTimer(void);



#endif /* MAIN_H_ */
