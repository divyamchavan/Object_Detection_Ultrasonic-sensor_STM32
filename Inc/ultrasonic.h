/*
 * ultrasonic.h
 *
 *  Created on: May 17, 2023
 *      Author: divya
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

void Delay_us(int);
void Tim3_Echo_Input_Capture();
void Trigger_init();
void Trigger();

#define SR_CC1IF    (1U<<1)

#endif /* ULTRASONIC_H_ */
