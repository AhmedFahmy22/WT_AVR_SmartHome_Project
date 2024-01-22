/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : ADC                                                 */
/* Version : 1.0                                                 */
/* Date    : 24 Dec 2023                                         */
/*****************************************************************/
#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

/*************Registers*************/
#define ADMUX    (*((volatile uint8*) 0x27))

#define ADMUX_BIT_REFS1 7
#define ADMUX_BIT_REFS0 6
#define ADMUX_BIT_ADLAR 5
#define ADMUX_BIT_MUX4  4
#define ADMUX_BIT_MUX3  3
#define ADMUX_BIT_MUX2  2
#define ADMUX_BIT_MUX1  1

#define ADMUX_REFS01_MASK 0b00111111
#define ADMUX_MUX04_MASK  0b11110000


#define ADCSRA   (*((volatile uint8*) 0x26))

#define ADCSRA_BIT_ADEN  7
#define ADCSRA_BIT_ADSC  6
#define ADCSRA_BIT_ADATE 5
#define ADCSRA_BIT_ADIF  4
#define ADCSRA_BIT_ADIE  3
#define ADCSRA_BIT_ADPS2 2
#define ADCSRA_BIT_ADPS1 1
#define ADCSRA_BIT_ADPS0 0
#define ADCSRA_ADPS02_MASK 0b11111000


#define ADCH     (*((volatile uint8*) 0x25))
#define ADCL     (*((volatile uint8*) 0x24))
#define ADCL_U16 (*((volatile uint16*) 0x24))


#define SFIOR    (*((volatile uint8*) 0x50))

#define SFIOR_BIT_ADTS2 7
#define SFIOR_BIT_ADTS1 6
#define SFIOR_BIT_ADTS0 5

#define SFIOR_ADTS02_MASK 0b00011111

/*************Configuration Macros*************/
#define ADC_STATE_DISABLE 0
#define ADC_STATE_ENABLE  1

#define ADC_REFERENCE_VOLTAGE_AREF 0
#define ADC_REFERENCE_VOLTAGE_AVCC 1
#define ADC_REFERENCE_VOLTAGE_256  3

#define ADC_PRESCALER_2   1
#define ADC_PRESCALER_4   2
#define ADC_PRESCALER_8   3
#define ADC_PRESCALER_16  4
#define ADC_PRESCALER_32  5
#define ADC_PRESCALER_64  6
#define ADC_PRESCALER_128 7

#define ADC_DATA_ADJUST_RIGHT 0
#define ADC_DATA_ADJUST_LEFT  1

#define ADC_AUTO_TRIGGER_DISABLE 0
#define ADC_AUTO_TRIGGER_ENABLE  1

#define ADC_INTERRUPT_DISABLE 0
#define ADC_INTERRUPT_ENABLE  1

#define ADC_AT_SOURCE_FREE_RUNNING      0
#define ADC_AT_SOURCE_ANALOG_COMPARATOR 1
#define ADC_AT_SOURCE_EXTINT_REQUEST0   2
#define ADC_AT_SOURCE_TIMER0_CMP        3
#define ADC_AT_SOURCE_TIMER0_OVF        4
#define ADC_AT_SOURCE_TIMER_CMP_B       5
#define ADC_AT_SOURCE_TIMER1_OVF        6
#define ADC_AT_SOURCE_TIMER1_CAP_EVENT  7

#endif
