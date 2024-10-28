# 2D-Auto-Test-Bed
<img src="Test System/system setup_1.jpg" width="1029" height="579">

## Overview

This project demonstrates a 2D testbed for precise movement across an X, Y mesh. It utilizes two stepper motors, two motor drivers, six sensors (three per axis), and an MSP430F5529 microcontroller. MATLAB is used to send control signals via UART to drive the motors in both directions.

## Components

- **MCU**: MSP430F5529
- **Motors**: 2 Stepper motors with 2 motor drivers
- **Sensors**: 6 sensors in total, with 3 dedicated to each axis (X and Y):
  - **1 Initial Position Sensor**: Detects the motor’s initial reference position.
  - **2 Safety Sensors**: Set safety limits, preventing the motor from moving beyond defined boundaries.
  <img src="Test System/Schematic Diagram.png" width="1029" height="579">
  
## System Workflow

1. **Motor Initialization**: The system begins by identifying the initial position of each motor using the initial position sensor.
2. **Movement**: MATLAB sends control signals via UART to the MCU, instructing the stepper motors to move along the X and Y axes.
3. **Safety Mechanism**: During movement, the safety sensors continuously monitor the motor's position to stop it from exceeding the safe limits.
4. **Control Feedback**: The motor movements are adjusted in real time based on feedback that send by the MCU after each step of the movement of the motor to ensure precise control and positioning.

