/*
 * pwm.c
 *
 *  Created on: Jul 8, 2024
 *      Author: afkar.mi
 */
    // Configure Timer A0 for PWM on STEP1_PIN (P2.0)
#include "pwm.h"

void pwm_init(void) {

    WDTCTL = WDTPW + WDTHOLD;                       // Stop WDT

    P2DIR |= MOTOR_x_STEP_PIN + MOTOR_y_STEP_PIN ;  // P2.0 and P2.4 output           // TA1.0 and TA2.1
    P2SEL |= MOTOR_x_STEP_PIN + MOTOR_y_STEP_PIN ;  // P2.0 and P2.4 options select   // TA1.0 and TA2.1

    TA1CCR0  = 1000;                                // TIMER A1 PWM Period/2
    TA2CCR0  = 1000;                                // TIMER A1 PWM Period/2
    TA1CCTL1 = OUTMOD_6;                            // TIMER A1 CCR1 toggle/set
    TA2CCTL1 = OUTMOD_6;                            // TIMER A2 CCR1 toggle/set

    Mx_speed = 0;                                   // TIMER A1 CCR1 PWM duty cycle
    My_speed = 0;                                   // TIMER A2 CCR1 PWM duty cycle

    TA1CTL = TASSEL_2 + MC_3 + TACLR;               // TIMER A1 SMCLK, up-down mode, clear TAR
    TA2CTL = TASSEL_2 + MC_3 + TACLR;               // TIMER A2 SMCLK, up-down mode, clear TAR

}




