/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : TIMER1                                              */
/* Version : 1.0                                                 */
/* Date    : 13 Jan 2024                                         */
/*****************************************************************/
#ifndef TIMER1_INTERFACE_H_
#define TIMER1_INTERFACE_H_

void TIMER1_voidInit(void);

void TIMER1_voidSetTimerValue(const uint16 u16TimerValueCpy);

void TIMER1_voidSetICR1Value(const uint16 u16ICR1ValueCpy);

void TIMER1_voidSetOCR1AValue(const uint16 u16ICR1ValueCpy);

void TIMER1_voidSetOCR1BValue(const uint16 u16ICR1ValueCpy);

uint16 TIMER1_u16GetTimerValue(void);

uint16 TIMER1_u16GetICR1Value(void);

uint16 TIMER1_u16GetOCR1AValue(void);

uint16 TIMER1_u16GetOCR1BValue(void);

void TIMER1_voidIntOvfEnable(void);

void TIMER1_voidIntOvfDisable(void);

void TIMER1_voidIntCmpAMatchEnable(void);

void TIMER1_voidIntCmpAMatchDisable(void);

void TIMER1_voidIntCmpBMatchEnable(void);

void TIMER1_voidIntCmpBMatchDisable(void);

tenuErrorStatus TIMER1_enuSetCallBackOvf(const pf pfOvfISRCpy);

tenuErrorStatus TIMER1_enuSetCallBackCmpMatchA(const pf pfCmpMatchAISRCpy);

tenuErrorStatus TIMER1_enuSetCallBackCmpMatchB(const pf pfCmpMatchBISRCpy);

#endif
