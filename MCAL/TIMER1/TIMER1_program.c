/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : TIMER1                                              */
/* Version : 1.0                                                 */
/* Date    : 13 Jan 2024                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "TIMER1_interface.h"
#include "TIMER1_private.h"
#include "TIMER1_config.h"

/*Global Variables*/
static pf pfOvfISRGlb = NULL_PTR;
static pf pfCmpMatchAISRGlb = NULL_PTR;
static pf pfCmpMatchBISRGlb = NULL_PTR;

/*Function Definitions*/
void TIMER1_voidInit(void)
{
    /*Waveform Generation Mode*/
    TCCR1A |= (TCCR1A & TCCR1A_WGM101_MASK) | ((TIMER1_WG_MODE&0b0011)<<TCCR1A_BIT_WGM10);
    TCCR1B |= (TCCR1B & TCCR1B_WGM123_MASK) | ((TIMER1_WG_MODE>>2)<<TCCR1B_BIT_WGM12);

    /*Compare Output Mode*/
    #if TIMER1_WG_MODE == TIMER1_WG_MODE_WG_MODE_NORMAL
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_PWM_PHASE_CORRECT_ICR1
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_PCPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_PCPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_PWM_PHASE_CORRECT_OCR1A
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_PCPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_PCPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_CTC_ICR1
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_CTC_OCR1A
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_NONPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_PWM_FAST_ICR1
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_FASTPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_FASTPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #elif TIMER1_WG_MODE == TIMER1_WG_MODE_PWM_FAST_OCR1A
    TCCR1A = (TCCR1A & TCCR1A_COM1A01_MASK) | (TIMER1_CMP_FASTPWM_OUTPUTA_MODE<<TCCR1A_BIT_COM1A0);
    TCCR1A = (TCCR1A & TCCR1A_COM1B01_MASK) | (TIMER1_CMP_FASTPWM_OUTPUTB_MODE<<TCCR1A_BIT_COM1B0);
    #endif

    /*Clock Prescaler*/
    TCCR1B = (TCCR1B & TCCR1B_CS103_MASK) | (TIMER1_CLK_PRESCALER);

    /*Interrupts*/
    #if TIMER1_INTERRUPT_OVF_MODE == TIMER1_INTERRUPT_OVF_MODE_DISABLE
    CLR_BIT(TIMSK, TIMSK_BIT_TOIE1);
    #elif TIMER1_WG_MODE_INTERRUPT_OVF_MODE == TIMER1_WG_MODE_INTERRUPT_OVF_MODE_ENABLE
    SET_BIT(TIMSK, TIMSK_BIT_TOIE1);
    #endif

    #if TIMER1_INTERRUPT_CMA_MODE == TIMER1_INTERRUPT_CMA_MODE_DISABLE
    CLR_BIT(TIMSK, TIMSK_BIT_OCIE1A);
    #elif TIMER1_INTERRUPT_CMA_MODE == TIMER1_INTERRUPT_CMA_MODE_ENABLE
    SET_BIT(TIMSK, TIMSK_BIT_OCIE1A);
    #endif

    #if TIMER1_INTERRUPT_CMB_MODE == TIMER1_INTERRUPT_CMB_MODE_DISABLE
    CLR_BIT(TIMSK, TIMSK_BIT_OCIE1B);
    #elif TIMER1_INTERRUPT_CMB_MODE == TIMER1_INTERRUPT_CMB_MODE_ENABLE
    SET_BIT(TIMSK, TIMSK_BIT_OCIE1B);
    #endif

}

void TIMER1_voidSetTimerValue(const uint16 u16TimerValueCpy)
{
    TCNT1L_U16 = u16TimerValueCpy;
}

void TIMER1_voidSetICR1Value(const uint16 u16ICR1ValueCpy)
{
    ICR1L_U16 = u16ICR1ValueCpy;
}

void TIMER1_voidSetOCR1AValue(const uint16 u16OCR1AValueCpy)
{
    OCR1AL_U16 = u16OCR1AValueCpy;
}

void TIMER1_voidSetOCR1BValue(const uint16 u16OCR1BValueCpy)
{
    OCR1BL_U16 = u16OCR1BValueCpy;
}

uint16 TIMER1_u16GetTimerValue(void)
{
    return TCNT1L_U16;
}

uint16 TIMER1_u16GetICR1Value(void)
{
    return ICR1L_U16;
}

uint16 TIMER1_u16GetOCR1AValue(void)
{
    return OCR1AL_U16;
}

uint16 TIMER1_u16GetOCR1BValue(void)
{
    return OCR1BL_U16;
}

void TIMER1_voidIntOvfEnable(void)
{
    SET_BIT(TIMSK,TIMSK_BIT_TOIE1);
}

void TIMER1_voidIntOvfDisable(void)
{
    CLR_BIT(TIMSK,TIMSK_BIT_TOIE1);
}

void TIMER1_voidIntCmpAMatchEnable(void)
{
    SET_BIT(TIMSK,TIMSK_BIT_OCIE1A);
}

void TIMER1_voidIntCmpAMatchDisable(void)
{
    CLR_BIT(TIMSK,TIMSK_BIT_OCIE1A);
}

void TIMER1_voidIntCmpBMatchEnable(void)
{
    SET_BIT(TIMSK,TIMSK_BIT_OCIE1B);
}

void TIMER1_voidIntCmpBMatchDisable(void)
{
    CLR_BIT(TIMSK,TIMSK_BIT_OCIE1B);
}

tenuErrorStatus TIMER1_enuSetCallBackOvf(const pf pfOvfISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfOvfISRCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfOvfISRGlb = pfOvfISRCpy;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus TIMER1_enuSetCallBackCmpMatchA(const pf pfCmpMatchAISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfCmpMatchAISRCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfCmpMatchAISRGlb = pfCmpMatchAISRCpy;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus TIMER1_enuSetCallBackCmpMatchB(const pf pfCmpMatchBISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfCmpMatchBISRCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfCmpMatchBISRGlb = pfCmpMatchBISRCpy;
    }

    return enuErrorStatLoc;
}


void __vector_7 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_7 (void){
    if(pfCmpMatchAISRGlb!=NULL_PTR)
    {
        pfCmpMatchAISRGlb();
    }
}

void __vector_8 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_8 (void){
    if(pfCmpMatchBISRGlb!=NULL_PTR)
    {
        pfCmpMatchBISRGlb();
    }
}

void __vector_9 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_9 (void){
    if(pfOvfISRGlb!=NULL_PTR)
    {
        pfOvfISRGlb();
    }
}
