/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : TIMER0                                              */
/* Version : 1.0                                                 */
/* Date    : 24 Dec 2023                                         */
/*****************************************************************/
#ifndef TIMER0_PRIVATE_H_
#define TIMER0_PRIVATE_H_

/*****Registers*****/
#define TCCR0 (*((volatile uint8*)0x53))
#define TCCR0_BIT_FOC0  7
#define TCCR0_BIT_WGM00 6
#define TCCR0_BIT_COM01 5
#define TCCR0_BIT_COM00 4
#define TCCR0_BIT_WGM01 3
#define TCCR0_BIT_CS02  2
#define TCCR0_BIT_CS01  1
#define TCCR0_BIT_CS00  0

#define TCCR0_CS002_MASK  0b11111000
#define TCCR0_COM001_MASK 0b11001111

#define TCNT0 (*((volatile uint8*)0x52))
#define OCR0 (*((volatile uint8*)0x5C))

#define TIMSK (*((volatile uint8*)0x59))
#define TIMSK_BIT_OCIE0 1
#define TIMSK_BIT_TOIE0 0

#define TIFR (*((volatile uint8*)0x58))

#define SFIOR (*((volatile uint8*)0x50))
#define SFIOR_BIT_PSR10 0

/*****Config Macros*****/
#define TIMER0_WG_MODE_NORMAL            0
#define TIMER0_WG_MODE_PWM_PHASE_CORRECT 1
#define TIMER0_WG_MODE_CTC               2
#define TIMER0_WG_MODE_PWM_FAST          3

#define TIMER0_CMP_NONPWM_OUTPUT_MODE_OFF      0
#define TIMER0_CMP_NONPWM_OUTPUT_MODE_TOGGLE   1
#define TIMER0_CMP_NONPWM_OUTPUT_MODE_CLEAR    2
#define TIMER0_CMP_NONPWM_OUTPUT_MODE_SET      3

#define TIMER0_CMP_FASTPWM_OUTPUT_MODE_OFF              0
#define TIMER0_CMP_FASTPWM_OUTPUT_MODE_CLEAR_CM_SET_TOP 2
#define TIMER0_CMP_FASTPWM_OUTPUT_MODE_SET_CM_CLEAR_TOP 3

#define TIMER0_CMP_PCPWM_OUTPUT_MODE_OFF                0
#define TIMER0_CMP_PCPWM_OUTPUT_MODE_CLEAR_UP_SET_DOWN  2
#define TIMER0_CMP_PCPWM_OUTPUT_MODE_SET_UP_CLEAR_DOWN  3

#define TIMER0_CLK_PRESCALER_TIMER_OFF             0
#define TIMER0_CLK_PRESCALER_1                     1
#define TIMER0_CLK_PRESCALER_8                     2
#define TIMER0_CLK_PRESCALER_64                    3
#define TIMER0_CLK_PRESCALER_256                   4
#define TIMER0_CLK_PRESCALER_1024                  5
#define TIMER0_CLK_PRESCALER_EXTERNAL_FALLING_EDGE 6
#define TIMER0_CLK_PRESCALER_EXTERNAL_RISING_EDGE  7

#define TIMER0_INTERRUPT_OVF_MODE_DISABLE 0
#define TIMER0_INTERRUPT_OVF_MODE_ENABLE  1

#define TIMER0_INTERRUPT_CM_MODE_DISABLE 0
#define TIMER0_INTERRUPT_CM_MODE_ENABLE  1

#define TIMER0_OC0_PORT DIO_PORTB
#define TIMER0_OC0_PIN  DIO_PIN3

#endif
