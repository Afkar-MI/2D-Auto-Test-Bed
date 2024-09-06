/*
 * sensor.c
 *
 *  Created on: Jul 17, 2024
 *      Author: afkar.mi
 */

#include "sensor.h"

void sensor_init(void){

        //Set sensors of Motor X pins as input
        P1DIR &= ~(SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN );
        P1REN |= SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN ;
        P1OUT |= SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN ;

        //Set sensors of Motor Y pins as input
        P1DIR &= ~(SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN );
        P1REN |= SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN ;
        P1OUT |= SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN ;

        // Configure interrupts for sensors of Motor X
        P1IE  |= SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN ;        // Enable interrupt for sensor pins
        P1IES |= SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN ;        // Interrupt on high-to-low transition
        P1IFG &= ~(SN_Mx_RIGHT_PIN + SN_Mx_LEFT_PIN + SN_Mx_INIT_PIN);      // Interrupt on high-to-low transition

        // Configure interrupts for sensors of Motor Y
        P1IE  |= SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN ;        // Enable interrupt for sensor pins
        P1IES |= SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN ;        // Interrupt on high-to-low transition
        P1IFG &= ~(SN_My_RIGHT_PIN + SN_My_LEFT_PIN + SN_My_INIT_PIN );     // Interrupt on high-to-low transition


}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    //------------Motor X Sensor--------
    if ((P1IFG & SN_Mx_RIGHT_PIN)) {
        Mx_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_Mx_RIGHT_PIN;      // Clear interrupt flag
        flag_snr_stop = 1;              // Enable the stop flag
    }

    if ((P1IFG & SN_Mx_LEFT_PIN)) {
        Mx_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_Mx_LEFT_PIN;       // Clear interrupt flag
        flag_snr_stop = 1;              // Enable the stop flag
    }

    // Initial position sensor
    if ((P1IFG & SN_Mx_INIT_PIN)) {
        Mx_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_Mx_INIT_PIN;       // Clear interrupt flag
        flag_SN_Mx_INIT_stop = 1;       // Enable the stop flag
    }

    //------------Motor Y Sensor--------
    if ((P1IFG & SN_My_RIGHT_PIN)) {
        My_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_My_RIGHT_PIN;      // Clear interrupt flag
        flag_snr_stop = 1;              // Enable the stop flag
    }

    if ((P1IFG & SN_My_LEFT_PIN)) {
        My_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_My_LEFT_PIN;       // Clear interrupt flag
        flag_snr_stop = 1;              // Enable the stop flag
    }

    // Initial position sensor
    if ((P1IFG & SN_My_INIT_PIN)) {
        My_speed = 0;                    // Stop Motor
        P1IFG &= ~SN_My_INIT_PIN;       // Clear interrupt flag
        flag_SN_My_INIT_stop = 1;       // Enable the stop flag
    }

}
