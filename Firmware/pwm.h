/*
 * pwm.h
 *
 *  Created on: Jul 8, 2024
 *      Author: afkar.mi
 */

#ifndef PWM_H_
#define PWM_H_

#include <msp430.h>
#include "driverlib.h"

#define MOTOR_x_STEP_PIN  BIT0                  // P2.0    // MotorX // X Pulse
#define MOTOR_y_STEP_PIN  BIT4                  // P2.4    // MotorY // Y Pulse

#define Mx_speed TA1CCR1                        // Motor X Speed
#define My_speed TA2CCR1                        // Motor Y Speed

extern volatile unsigned int steps_Mx;          // Steps for Motor X
extern volatile unsigned int steps_My;          // Steps for Motor Y

extern volatile unsigned int flag_Mx_stp_end;   // Motor X steps end flag
extern volatile unsigned int Mx_Running ;       // Motor X running state

extern volatile unsigned int flag_My_stp_end;   // Motor Y steps end flag
extern volatile unsigned int My_Running ;       // Motor Y running state



void pwm_init(void);                            //initiate the PWM modules
/*void motor_status(void);*/
#endif /* PWM_H_ */
