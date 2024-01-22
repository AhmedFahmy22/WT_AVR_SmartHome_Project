/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : TIMER1                                              */
/* Version : 1.0                                                 */
/* Date    : 13 Jan 2024                                         */
/*****************************************************************/
#ifndef TIMER1_PRIVATE_H_
#define TIMER1_PRIVATE_H_

/*****Registers*****/
#define TCCR1A (*((volatile uint8*)0x4F))
#define TCCR1A_BIT_COM1A0 6
#define TCCR1A_BIT_COM1B0 4
#define TCCR1A_BIT_FOC1A  3
#define TCCR1A_BIT_FOC1B  2
#define TCCR1A_BIT_WGM11  1
#define TCCR1A_BIT_WGM10  0
#define TCCR1A_COM1A01_MASK 0b00111111
#define TCCR1A_COM1B01_MASK 0b11001111
#define TCCR1A_WGM101_MASK  0b11111100

#define TCCR1B (*((volatile uint8*)0x4E))
#define TCCR1B_BIT_ICNC1 7
#define TCCR1B_BIT_ICES1 6
#define TCCR1B_BIT_WGM13 4
#define TCCR1B_BIT_WGM12 3
#define TCCR1B_BIT_CS12  2
#define TCCR1B_BIT_CS11  1
#define TCCR1B_BIT_CS10  0

#define TCCR1B_CS103_MASK   0b11111000
#define TCCR1B_WGM123_MASK  0b11100111

#define TCNT1H (*((volatile uint8*)0x4D))
#define TCNT1L (*((volatile uint8*)0x4C))
#define TCNT1L_U16  (*((volatile uint16*)0x4C))

#define OCR1AH (*((volatile uint8*)0x4B))
#define OCR1AL (*((volatile uint8*)0x4A))
#define OCR1AL_U16  (*((volatile uint16*)0x4A))

#define OCR1BH (*((volatile uint8*)0x49))
#define OCR1BL (*((volatile uint8*)0x48))
#define OCR1BL_U16  (*((volatile uint16*)0x48))

#define ICR1H (*((volatile uint8*)0x47))
#define ICR1L (*((volatile uint8*)0x46))
#define ICR1L_U16  (*((volatile uint16*)0x46))

#define TIMSK (*((volatile uint8*)0x59))
#define TIMSK_BIT_OCIE1A 4
#define TIMSK_BIT_OCIE1B 3
#define TIMSK_BIT_TOIE1  2

#define TIFR (*((volatile uint8*)0x58))

#define SFIOR (*((volatile uint8*)0x50))
#define SFIOR_BIT_PSR10 0

/*****Config Macros*****/
#define TIMER1_WG_MODE_NORMAL                          0
#define TIMER1_WG_MODE_PWM_PHASE_CORRECT_8BIT          1
#define TIMER1_WG_MODE_PWM_PHASE_CORRECT_9BIT          2
#define TIMER1_WG_MODE_PWM_PHASE_CORRECT_10BIT         3
#define TIMER1_WG_MODE_CTC_OCR1A                       4
#define TIMER1_WG_MODE_PWM_FAST_8BIT                   5
#define TIMER1_WG_MODE_PWM_FAST_9BIT                   6
#define TIMER1_WG_MODE_PWM_FAST_10BIT                  7
#define TIMER1_WG_MODE_PWM_PHASE_FREQ_CORRECT_ICR1     8
#define TIMER1_WG_MODE_PWM_PHASE_FREQ_CORRECT_OCR1A    9
#define TIMER1_WG_MODE_PWM_PHASE_CORRECT_ICR1          10
#define TIMER1_WG_MODE_PWM_PHASE_CORRECT_OCR1A         11
#define TIMER1_WG_MODE_CTC_ICR1                        12
#define TIMER1_WG_MODE_PWM_FAST_ICR1                   14
#define TIMER1_WG_MODE_PWM_FAST_OCR1A                  15

#define TIMER1_CMP_NONPWM_OUTPUTA_MODE_OFF      0
#define TIMER1_CMP_NONPWM_OUTPUTA_MODE_TOGGLE   1
#define TIMER1_CMP_NONPWM_OUTPUTA_MODE_CLEAR    2
#define TIMER1_CMP_NONPWM_OUTPUTA_MODE_SET      3

#define TIMER1_CMP_FASTPWM_OUTPUTA_MODE_OFF              0
#define TIMER1_CMP_FASTPWM_OUTPUTA_MODE_CLEAR_CM_SET_TOP 2
#define TIMER1_CMP_FASTPWM_OUTPUTA_MODE_SET_CM_CLEAR_TOP 3

#define TIMER1_CMP_PCPWM_OUTPUTA_MODE_OFF                0
#define TIMER1_CMP_PCPWM_OUTPUTA_MODE_CLEAR_UP_SET_DOWN  2
#define TIMER1_CMP_PCPWM_OUTPUTA_MODE_SET_UP_CLEAR_DOWN  3

#define TIMER1_CMP_NONPWM_OUTPUTB_MODE_OFF      0
#define TIMER1_CMP_NONPWM_OUTPUTB_MODE_TOGGLE   1
#define TIMER1_CMP_NONPWM_OUTPUTB_MODE_CLEAR    2
#define TIMER1_CMP_NONPWM_OUTPUTB_MODE_SET      3

#define TIMER1_CMP_FASTPWM_OUTPUTB_MODE_OFF              0
#define TIMER1_CMP_FASTPWM_OUTPUTB_MODE_CLEAR_CM_SET_TOP 2
#define TIMER1_CMP_FASTPWM_OUTPUTB_MODE_SET_CM_CLEAR_TOP 3

#define TIMER1_CMP_PCPWM_OUTPUTB_MODE_OFF                0
#define TIMER1_CMP_PCPWM_OUTPUTB_MODE_CLEAR_UP_SET_DOWN  2
#define TIMER1_CMP_PCPWM_OUTPUTB_MODE_SET_UP_CLEAR_DOWN  3

#define TIMER1_CLK_PRESCALER_TIMER_OFF             0
#define TIMER1_CLK_PRESCALER_1                     1
#define TIMER1_CLK_PRESCALER_8                     2
#define TIMER1_CLK_PRESCALER_64                    3
#define TIMER1_CLK_PRESCALER_256                   4
#define TIMER1_CLK_PRESCALER_1024                  5
#define TIMER1_CLK_PRESCALER_EXTERNAL_FALLING_EDGE 6
#define TIMER1_CLK_PRESCALER_EXTERNAL_RISING_EDGE  7

#define TIMER1_INTERRUPT_OVF_MODE_DISABLE 0
#define TIMER1_INTERRUPT_OVF_MODE_ENABLE  1

#define TIMER1_INTERRUPT_CMA_MODE_DISABLE 0
#define TIMER1_INTERRUPT_CMA_MODE_ENABLE  1

#define TIMER1_INTERRUPT_CMB_MODE_DISABLE 0
#define TIMER1_INTERRUPT_CMB_MODE_ENABLE  1

#define TIMER1_OC1A_PORT DIO_PORTD
#define TIMER1_OC1A_PIN  DIO_PIN5

#define TIMER1_OC1B_PORT DIO_PORTD
#define TIMER1_OC1B_PIN  DIO_PIN4


#endif
