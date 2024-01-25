/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : ADC                                                 */
/* Version : 1.0                                                 */
/* Date    : 24 Dec 2023                                         */
/*****************************************************************/
#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#define ADC_STATE ADC_STATE_ENABLE
/* Options
* ADC_STATE_DISABLE
* ADC_STATE_ENABLE
*/


#define ADC_REFERENCE_VOLTAGE ADC_REFERENCE_VOLTAGE_256
/* Options
* ADC_REFERENCE_VOLTAGE_AREF
* ADC_REFERENCE_VOLTAGE_AVCC
* ADC_REFERENCE_VOLTAGE_256
*/


#define ADC_PRESCALER ADC_PRESCALER_2
/* Options
* ADC_PRESCALER_2
* ADC_PRESCALER_4
* ADC_PRESCALER_8
* ADC_PRESCALER_16
* ADC_PRESCALER_32
* ADC_PRESCALER_64
* ADC_PRESCALER_128
*/


#define ADC_DATA_ADJUST ADC_DATA_ADJUST_RIGHT
/* Options
* ADC_DATA_ADJUST_RIGHT
* ADC_DATA_ADJUST_LEFT
*/


#define ADC_AUTO_TRIGGER ADC_AUTO_TRIGGER_DISABLE
/* Options
* ADC_AUTO_TRIGGER_DISABLE
* ADC_AUTO_TRIGGER_ENABLE
*/


#define ADC_INTERRUPT ADC_INTERRUPT_ENABLE
/* Options
* ADC_INTERRUPT_DISABLE
* ADC_INTERRUPT_ENABLE
*/


#define ADC_AT_SOURCE  ADC_AT_SOURCE_FREE_RUNNING
/* Options
* ADC_AT_SOURCE_FREE_RUNNING
* ADC_AT_SOURCE_ANALOG_COMPARATOR
* ADC_AT_SOURCE_EXTINT_REQUEST0
* ADC_AT_SOURCE_TIMER0_CMP
* ADC_AT_SOURCE_TIMER0_OVF
* ADC_AT_SOURCE_TIMER_CMP_B
* ADC_AT_SOURCE_TIMER1_OVF
* ADC_AT_SOURCE_TIMER1_CAP_EVENT
*/

#endif
