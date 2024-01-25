# WT_AVR_SmartHome_Project

**Smart Home Project**
**(AVR ATMEGA32)**

![proteus_print_page-0001](https://github.com/AhmedFahmy22/WT_AVR_SmartHome_Project/assets/116200429/87d3e085-ca46-42e3-aba1-2f4f74bcff7d)


**Description**

Smart home controlled and automated by user using:
- 3 buttons (Up, Down Enter)
- Android phone Bluetooth module by sending ('U' or 'u', 'D' or 'd', 'E' or 'e')

At first, the user will be asked to enter PIN (0000 default) before accessing the
application:

1.  Status Display (Temperature & Light)

2.  Timer / Alarm

3.  Fan / DC motor Control

4.  Light Control

5.  Servo motor Control

6.  Change PIN

The user can configure/control these components:

\- Fan (DC Motor) with (Temperature sensor or Timer)

\- Light source (LED) with (Light sensor or Timer)

\- Buzzer (Alarm) with (Timer)

\- Servo motor

\- Change PIN (Saved in EEPROM)

**Drivers**

1.  MCAL

    1.  DIO

    2.  ADC

    3.  Timers

    4.  External Interrupts

    5.  UART

    6.  I2C

2.  HAL

    1.  LEDs

    2.  LCD

    3.  Servo motor

    4.  DC motor

    5.  Buzzer

    6.  Temp. sensor

    7.  Light sensor

    8.  External EEPROM


**Pins configuration**

| Pin No. | Direction | Mode       |                    |
|---------|-----------|------------|--------------------|
| A0      | INPUT     | ADC0       | LM35               |
| A1      | INPUT     | ADC1       | LDR                |
| A2      | OUTPUT    | PA2 (DIO)  | LED (Light Source) |
| A3      | OUTPUT    | PA3 (DIO)  | DC Motor           |
| A4      | OUTPUT    | PA4 (DIO)  | Buzzer             |
| A5      | OUTPUT    | PA5(DIO)   | LED (Error)        |
| A6      | ---       | ---        | ---                |
| A7      | ---       | ---        | ---                |
| B0      | ---       | ---        | ---                |
| B1      | ---       | ---        | ---                |
| B2      | INPUT     | INT2       | Button 3           |
| B3      | OUTPUT    | PB3 (DIO)  | LCD Data Pin 0     |
| B4      | OUTPUT    | PB4 (DIO)  | LCD Data Pin 1     |
| B5      | OUTPUT    | PB5 (DIO)  | LCD Data Pin 2     |
| B6      | OUTPUT    | PB6 (DIO)  | LCD Data Pin 3     |
| B7      | OUTPUT    | PB7 (DIO)  | LCD Data Pin 4     |
| C0      | OUTPUT    | SCL (I2C)  | EEPROM CLK         |
| C1      | ---       | SDA (I2C)  | EEPROM DATA        |
| C2      | OUTPUT    | PC2 (DIO)  | LCD RS Pin         |
| C3      | OUTPUT    | PC3 (DIO)  | LCD RW Pin         |
| C4      | OUTPUT    | PC4 (DIO)  | LCD E Pin          |
| C5      | OUTPUT    | PC5 (DIO)  | LCD Data Pin 5     |
| C6      | OUTPUT    | PC6 (DIO)  | LCD Data Pin 6     |
| C7      | OUTPUT    | PC7 (DIO)  | LCD Data Pin 7     |
| D0      | INPUT     | RXD (UART) | Bluetooth Receive  |
| D1      | OUTPUT    | TXD (UART) | Bluetooth Transmit |
| D2      | INPUT     | INT0       | Button 1           |
| D3      | INPUT     | INT1       | Button 2           |
| D4      | ---       | ---        | ---                |
| D5      | OUTPUT    | PD5 (DIO)  | Servo Motor        |
| D6      | ---       | ---        | ---                |
| D7      | ---       | ---        | ---                |
