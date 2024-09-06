/*   Wiring Diagram

//                     MSP430F5529
//                 -------------------
//                 |             P1.0| - X S
//   X Pulse     - |P2.0         P1.1| - X E
//   X Direction - |P2.1         P1.2| - X I
//                 |             P1.3| - Y S
//   Y Pulse     - |P2.4         P1.4| - Y E
//   Y Direction - |P2.5         P1.5| - Y I
//                 |                 |
//                 |     P4.4/UCA1TXD| - RX CH340
//                 |     P4.5/UCA1RXD| - TX CH340

//
   UART receive data format
   {PositionByte, Motor Selection, Direction, 4 Steps Byte}

//----------------------------------------------------------------
  ----------------------------------------------------------------
*/
#include <msp430.h>
#include "driverlib.h"
#include <math.h>

#include "uart.h"
#include "clock.h"
#include "pwm.h"
#include "sensor.h"

uint8_t uart_recv_buf[UART_RECV_LENGTH]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};// Buffer to store data received
uint8_t flag_uart_recv = 0;                         // UART Receive flag
uint8_t Flag_My = 0xFF;                             // acknowledgment after Motor Y movement
uint8_t Flag_Mx = 0xAA;                             // acknowledgment after Motor X movement

// Define the header
#define M_Zero          0x30                        // header for Zeroth position
#define DirLeft         0x20                        // header for left direction // direction == low
#define DirRight        0x40                        // header for right direction // direction == high
#define StartPos        0x50                        // header for initial position
#define Mtr_x           0x60                        // header for Motor X
#define Mtr_y           0X70                        // header for Motor Y


//define the pin connections for motor x
#define Mx_DIR_PIN   BIT1                           // P2.1  // X Direction
#define My_DIR_PIN   BIT5                           // P2.5  // Y Direction
//Define the PWM duty cycle as speed parameter
#define Mx_speed    TA1CCR1                         // Motor X Speed
#define My_speed    TA2CCR1                         // Motor Y Speed

//define the sensors state ( LOW when it detects the motor)
#define S_Mx_I      P1IN & SN_Mx_INIT_PIN           // Motor X Initial sensor state
#define S_Mx_L      P1IN & SN_Mx_LEFT_PIN           // Motor X Left sensor state
#define S_Mx_R      P1IN & SN_Mx_RIGHT_PIN          // Motor X Right sensor state

#define S_My_I      P1IN & SN_My_INIT_PIN           // Motor Y Initial sensor state
#define S_My_L      P1IN & SN_My_LEFT_PIN           // Motor Y Left sensor state
#define S_My_R      P1IN & SN_My_RIGHT_PIN          // Motor Y Right sensor state

#define safe_state  ((S_Mx_L)&&(S_Mx_R)&&(S_My_L)&&(S_My_R))  //check whether all the safe sensor are clear.

// Sensors' Flag
volatile unsigned int flag_snr_stop = 0;            // Disable Flag to stop the whole system (Safety Precaution)
volatile unsigned int flag_SN_Mx_INIT_stop = 0;     // Disable initial position of Motor X
volatile unsigned int flag_SN_My_INIT_stop = 0;     // Disable initial position of Motor Y

//Initiate the steps and other flags
volatile unsigned int steps_Mx = 0;                 // Steps for Motor X
volatile unsigned int steps_My = 0;                 // Steps for Motor Y

volatile unsigned int flag_Mx_stp_end = 0;          // Motor X steps end flag
volatile unsigned int Mx_Running = 0;               // Motor X running state

volatile unsigned int flag_My_stp_end = 0;          // Motor Y steps end flag
volatile unsigned int My_Running = 0;               // Motor Y running state

unsigned int M_steps  = 0;                          // Motor steps received from PC
uint8_t   M_Dir = 0x00;                             // Motor Direction received from PC
uint8_t   M_Sel = 0x00;                             // Motor Selection byte
uint8_t   M_Init= 0x00;                             // Move the motor to initial state.

void moveMotor(unsigned char motor, unsigned int steps, unsigned char direction);
void returnToZero(void);
int stepsCal();
void gpio_init(void);



int main(void)
{
         base_init();
         gpio_init();
         __delay_cycles(250000);                  // Delay for DCO to settle
         uart_init();
         pwm_init();
         sensor_init();

         __bis_SR_register(GIE);                    // open interrupt

         if (safe_state)
                 {

             returnToZero();                        // set the motor to initial position

             while(!flag_snr_stop)
               {

                 if(flag_uart_recv == 1)

                     {

                     M_steps = stepsCal();
                     M_Init  = uart_recv_buf[0];
                     M_Sel   = uart_recv_buf[1];     // Get the Motor (X?Y)
                     M_Dir   = uart_recv_buf[2];     // Get the Direction

                     // Move motors based on received data
                     if (M_Init == M_Zero){
                         returnToZero();
                     }
                     else{
                         moveMotor(M_Sel ,M_steps, M_Dir);          // move motor
                     }


                     if (flag_Mx_stp_end == 1){
                         uart_send_data(Flag_Mx);
                         }

                     else if (flag_My_stp_end == 1){
                         uart_send_data(Flag_My);
                         }

                     flag_uart_recv = 0;
                      flag_Mx_stp_end = 0;
                      flag_My_stp_end = 0;


                     }

                }
         }


}



//------------------------GPIO pins initiate------------------//
void gpio_init(void){

    //Pin Config Motors
    P2DIR |= Mx_DIR_PIN + My_DIR_PIN ;       //Motor x and Motor Y

}


//-------------------Return the stepper motor to initial position-----------------------
void returnToZero(void){

    // First return the Motor X to initial position
    while(!flag_SN_My_INIT_stop){

        if (flag_SN_My_INIT_stop || !(S_My_I)) {
            My_speed = 0;
            break;
        } else {
            P2OUT |= My_DIR_PIN;
            My_speed = 500;
            My_Running = 1;
        }
    }

    My_speed = 0;
    flag_SN_My_INIT_stop = 0;
    __delay_cycles(10000);

    while (!flag_SN_Mx_INIT_stop ) {

        if (flag_SN_Mx_INIT_stop || !(S_Mx_I)) {
            Mx_speed = 0;
            break;

        } else {
            P2OUT |= Mx_DIR_PIN;
            Mx_speed = 500;
            Mx_Running = 1;
        }

    }

    Mx_speed = 0;                          // set the motor Y to stable
    flag_SN_Mx_INIT_stop=0;

}

//----------------Calculate the steps from the data received from the computer------------------
// convert the hexadecimal data into integer value
int stepsCal(){
    int steps_count;
    int i;
    for ( i = 3; i < UART_RECV_LENGTH; ++i) {
          steps_count = (steps_count << 8) | uart_recv_buf[i];
      }
    return steps_count;
}

//--------------------moving the stepper motor---------------------

void moveMotor(uint8_t motor, unsigned int steps, uint8_t direction) {
     switch (motor) {
        case Mtr_x: // Motor X

            steps_Mx = steps;
            Mx_Running = 1;
            if (direction == DirRight) {
                P2OUT |= Mx_DIR_PIN;   // Right Direction
            }
            else if (direction == DirLeft) {
                P2OUT &= ~Mx_DIR_PIN;  // Left Direction
            }
         // Start PWM for Motor 1
            int i;
            for( i=0; i<steps_Mx; i++){
                if(flag_snr_stop){
                    break;
                }
                __delay_cycles(1000);
                Mx_speed  = 500;
            }
            Mx_speed  = 0;
            flag_Mx_stp_end = 1;  // Set flag for Motor X step end
            Mx_Running = 0;       // Motor X stopped
            break;

        case Mtr_y: // Motor Y

            steps_My = steps;
            My_Running = 1;
            if (direction == DirRight) {
                P2OUT |= My_DIR_PIN;   // Right Direction
            }
            else if (direction == DirLeft) {
                P2OUT &= ~My_DIR_PIN;  // Left Direction
            }

            int j;// Start PWM for Motor 1
            for( j=0; j<steps_My; j++){
                if(flag_snr_stop){
                      break;
                    }
                __delay_cycles(1000);
                 My_speed  = 500;
            }
            My_speed  = 0;
            flag_My_stp_end = 1;  // Set flag for Motor Y step end
            My_Running = 0;       // Motor Y stopped
            break;

    }
}



