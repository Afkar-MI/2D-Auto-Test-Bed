/*
 * sensor.h
 *
 *  Created on: Jul 17, 2024
 *      Author: afkar.mi
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <msp430.h>
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>

#define SN_Mx_RIGHT_PIN     BIT0         // Motor X Right Sensor            P1.0    // X S  // Safety Sensor
#define SN_Mx_LEFT_PIN      BIT1         // Motor X Left Sensor             P1.1    // X E  // Safety Sensor
#define SN_Mx_INIT_PIN      BIT2         // Motor X Initial position sensor P1.2    // X I

#define SN_My_RIGHT_PIN     BIT3         // Motor Y Right Sensor            P1.3    // Y S  // Safety Sensor
#define SN_My_LEFT_PIN      BIT4         // Motor Y Left Sensor             P1.4    // Y E  // Safety Sensor
#define SN_My_INIT_PIN      BIT5         // Motor Y Initial Position Sensor P1.5    // Y I

extern volatile unsigned int flag_snr_stop;            // Flag to stop the whole system (Safety Precaution)
extern volatile unsigned int flag_SN_Mx_INIT_stop;     // Flag for initial position of Motor X
extern volatile unsigned int flag_SN_My_INIT_stop;     // Flag for initial position of Motor Y

#define Mx_speed TA1CCR1                        // Motor X Speed
#define My_speed TA2CCR1                        // Motor Y Speed

void sensor_init(void);

#endif /* SENSOR_H_ */


