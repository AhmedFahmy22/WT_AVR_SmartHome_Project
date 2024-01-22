/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : TIMER0                                              */
/* Version : 1.0                                                 */
/* Date    : 24 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "../DIO/DIO_interface.h"

#include "TIMER0_interface.h"
#include "TIMER0_private.h"
#include "TIMER0_config.h"

/*Global Variables*/
pf pfOvfISRGlb = NULL_PTR;
pf pfCmpMatchISRGlb = NULL_PTR;
static uint32 u32OvfCntrGlb = 0;

/*Function Definitions*/
void TIMER0_voidInit(void)
{
    /*Waveform Generation Mode*/
    #if TIMER0_WG_MODE == TIMER0_WG_MODE_NORMAL
    CLR_BIT(TCCR0, TCCR0_BIT_WGM00);
    CLR_BIT(TCCR0, TCCR0_BIT_WGM01);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_PWM_PHASE_CORRECT
    SET_BIT(TCCR0, TCCR0_BIT_WGM00);
    CLR_BIT(TCCR0, TCCR0_BIT_WGM01);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_CTC
    CLR_BIT(TCCR0, TCCR0_BIT_WGM00);
    SET_BIT(TCCR0, TCCR0_BIT_WGM01);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_PWM_FAST
    SET_BIT(TCCR0, TCCR0_BIT_WGM00);
    SET_BIT(TCCR0, TCCR0_BIT_WGM01);
    #endif

    /*Compare Output Mode*/
    #if TIMER0_WG_MODE == TIMER0_WG_MODE_NORMAL
    TCCR0 = (TCCR0 & TCCR0_COM001_MASK) | (TIMER0_CMP_NONPWM_OUTPUT_MODE<<TCCR0_BIT_COM00);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_PWM_PHASE_CORRECT
    TCCR0 = (TCCR0 & TCCR0_COM001_MASK) | (TIMER0_CMP_FASTPWM_OUTPUT_MODE<<TCCR0_BIT_COM00);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_CTC
    TCCR0 = (TCCR0 & TCCR0_COM001_MASK) | (TIMER0_CMP_NONPWM_OUTPUT_MODE<<TCCR0_BIT_COM00);
    #elif TIMER0_WG_MODE == TIMER0_WG_MODE_PWM_FAST
    TCCR0 = (TCCR0 & TCCR0_COM001_MASK) | (TIMER0_CMP_PCPWM_OUTPUT_MODE<<TCCR0_BIT_COM00);
    #endif

    /*Direction of OC0 pin*/
    if((TCCR0 & ((uint8) ~TCCR0_COM001_MASK))!=0)
    {
        DIO_enuSetPinDirection(TIMER0_OC0_PORT, TIMER0_OC0_PIN, DIO_OUTPUT);
    }

    /*Clock Prescaler*/
    TCCR0 = (TCCR0 & TCCR0_CS002_MASK) | (TIMER0_CLK_PRESCALER);

    /*Interrupts*/
    #if TIMER0_INTERRUPT_OVF_MODE == TIMER0_INTERRUPT_OVF_MODE_DISABLE
    CLR_BIT(TIMSK, TIMSK_BIT_TOIE0);
    #elif TIMER0_INTERRUPT_OVF_MODE == TIMER0_INTERRUPT_OVF_MODE_ENABLE
    SET_BIT(TIMSK, TIMSK_BIT_TOIE0);
    #endif

    #if TIMER0_INTERRUPT_CM_MODE == TIMER0_INTERRUPT_CM_MODE_DISABLE
    CLR_BIT(TIMSK, TIMSK_BIT_OCIE0);
    #elif TIMER0_INTERRUPT_CM_MODE == TIMER0_INTERRUPT_CM_MODE_ENABLE
    SET_BIT(TIMSK, TIMSK_BIT_OCIE0);
    #endif

}

tenuErrorStatus TIMER0_enuEnable(const uint8 u8ClkPrescalerCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(u8ClkPrescalerCpy>7)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        TCCR0 = (TCCR0 & TCCR0_CS002_MASK) | (u8ClkPrescalerCpy);
    }
    return enuErrorStatLoc;
}

void TIMER0_voidSetTimerValue(const uint8 u8TimerValueCpy)
{
    TCNT0 = u8TimerValueCpy;
}

tenuErrorStatus TIMER0_enuGetTimerValue(uint8* const pu8TimerValueCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(pu8TimerValueCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        *pu8TimerValueCpy = TCNT0;
    }
    return enuErrorStatLoc;
}

void TIMER0_voidSetCmpMatchValue(const uint8 u8CmpMatchValueCpy)
{
    OCR0 = u8CmpMatchValueCpy;
}

void TIMER0_voidIntOvfEnable(void)
{
    SET_BIT(TIMSK, TIMSK_BIT_TOIE0);
}

void TIMER0_voidIntOvfDisable(void)
{
    CLR_BIT(TIMSK, TIMSK_BIT_TOIE0);
}

void TIMER0_voidIntCmpMatchEnable(void)
{
    SET_BIT(TIMSK, TIMSK_BIT_OCIE0);
}

void TIMER0_voidIntCmpMatchDisable(void)
{
    CLR_BIT(TIMSK, TIMSK_BIT_OCIE0);
}

tenuErrorStatus TIMER0_enuSetCallBackOvf(const pf pfOvfISRCpy)
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

tenuErrorStatus TIMER0_enuSetCallBackCmpMatch(const pf pfCmpMatchISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(pfCmpMatchISRCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfCmpMatchISRGlb = pfCmpMatchISRCpy;
    }
    return enuErrorStatLoc;
}

void TIMER0_voidDelay(uint32 u32OvfDelayCpy)
{
	if(TCNT0>150)
	{
		u32OvfDelayCpy++;
	}
	u32OvfDelayCpy+=2;
	u32OvfCntrGlb=0;
	while((u32OvfCntrGlb)!=u32OvfDelayCpy);
}

void __vector_10 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_10 (void){
	u32OvfCntrGlb++;
	if(pfCmpMatchISRGlb!=NULL_PTR)
    {
        pfCmpMatchISRGlb();
    }
}

void __vector_11 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_11 (void){

	if(pfOvfISRGlb!=NULL_PTR)
    {
        pfOvfISRGlb();
    }
}
