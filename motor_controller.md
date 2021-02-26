# [Pololu Jrk Series][1]
- Allows closed-loop speed control with analogue or pulse (tachometer) feedback.
- Position control only available with analogue feedback.
- Does NOT support quadrature encoders.

# [DFRobot DC Motor Driver HAT DFR0592][2]
- Allows feedback with quadrature encoder.
- Appropriate for the micro-metal DC motors.
  - Includes configurations like gear-ratio and automatic RPM calculation.
- Uses STM32 microcontroller, communication done using I2C.
- Doesn't explicitly say if it has integrated velocity/position control 
(my guess would be it does not, based on the functionality of the software
driver).

[1]: https://www.pololu.com/category/95/pololu-jrk-motor-controllers-with-feedback
[2]: https://wiki.dfrobot.com/DC_Motor_Driver_HAT_SKU_DFR0592
