/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : ADC                                                 */
/* Version : 1.0                                                 */
/* Date    : 24 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"

/*Global Variables*/
pf pfAdcISRGlb = NULL_PTR;

/*Function Definitions*/
void ADC_voidInit(void)
{
    /*Voltage Reference*/
    ADMUX = (ADMUX & ADMUX_REFS01_MASK) | (ADC_REFERENCE_VOLTAGE<<ADMUX_BIT_REFS0);

    /*Data Adjust*/
    #if ADC_DATA_ADJUST == ADC_DATA_ADJUST_RIGHT
    CLR_BIT(ADMUX,ADMUX_BIT_ADLAR);
    #elif ADC_DATA_ADJUST == ADC_DATA_ADJUST_LEFT
    SET_BIT(ADMUX,ADMUX_BIT_ADLAR);
    #endif

    /*Prescaler*/
    ADCSRA = (ADCSRA & ADCSRA_ADPS02_MASK) | ADC_PRESCALER;

    /*Auto Trigger*/
    #if ADC_AUTO_TRIGGER == ADC_AUTO_TRIGGER_ENABLE
    SET_BIT(ADCSRA, ADCSRA_BIT_ADATE);
        /*Source*/
    SFIOR = (SFIOR & SFIOR_ADTS02_MASK) | (ADC_AT_SOURCE<<SFIOR_BIT_ADTS0);
    #elif ADC_AUTO_TRIGGER == ADC_AUTO_TRIGGER_DISABLE
    CLR_BIT(ADCSRA, ADCSRA_BIT_ADATE);
    #endif

    /*Interrupt*/
    #if ADC_INTERRUPT == ADC_INTERRUPT_ENABLE
    SET_BIT(ADCSRA, ADCSRA_BIT_ADIE);
    #elif ADC_INTERRUPT == ADC_INTERRUPT_DISABLE
    CLR_BIT(ADCSRA, ADCSRA_BIT_ADIE);
    #endif

    /*ADC On or Off*/
    #if ADC_STATE == ADC_STATE_ENABLE
    SET_BIT(ADCSRA, ADCSRA_BIT_ADEN);
    #elif ADC_STATE == ADC_STATE_DISABLE
    CLR_BIT(ADCSRA, ADCSRA_BIT_ADEN);
    #endif
}

void ADC_voidEnable(void)
{
    SET_BIT(ADCSRA, ADCSRA_BIT_ADEN);
}

void ADC_voidDisable(void)
{
    CLR_BIT(ADCSRA, ADCSRA_BIT_ADEN);
}

tenuErrorStatus ADC_enuReadSynch (const uint8 u8ChannelCpy, uint16*const pu16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(u8ChannelCpy>ADC_CHANNEL_A7)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        /*Channel*/
        ADMUX = (ADMUX & ADMUX_MUX04_MASK) | u8ChannelCpy;

        /*Start Conversion*/
        SET_BIT(ADCSRA,ADCSRA_BIT_ADSC);
        while(GET_BIT(ADCSRA,ADCSRA_BIT_ADIF)==0);

        /*Read Data*/
        enuErrorStatLoc |= ADC_enuGetData(pu16DataCpy);
        SET_BIT(ADCSRA,ADCSRA_BIT_ADIF);
    }
    return enuErrorStatLoc;
}

tenuErrorStatus ADC_enuReadAsynch(uint8 u8ChannelCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(u8ChannelCpy>ADC_CHANNEL_A7)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        /*Channel*/
        ADMUX = (ADMUX & ADMUX_MUX04_MASK) | u8ChannelCpy;

        /*Start Conversion*/
        SET_BIT(ADCSRA,ADCSRA_BIT_ADSC);
    }
    return enuErrorStatLoc;
}

tenuErrorStatus ADC_enuGetData(uint16*const pu16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(pu16DataCpy == NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        *pu16DataCpy = ADCL_U16;
        #if ADC_DATA_ADJUST == ADC_DATA_ADJUST_LEFT
        *pu16DataCpy = (*pu16DataCpy)>>6;
        #endif
    }
    return enuErrorStatLoc;
}

tenuErrorStatus ADC_enuChangeChannel(const uint8 u8ChannelCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(u8ChannelCpy>ADC_CHANNEL_A7)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        ADMUX = (ADMUX & ADMUX_MUX04_MASK) | u8ChannelCpy;
    }
    return enuErrorStatLoc;
}

tenuErrorStatus ADC_enuChangeVoltRef(const uint8 u8RefVoltageCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(u8RefVoltageCpy>ADC_REFERENCE_VOLTAGE_256)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
    	ADMUX = (ADMUX & ADMUX_REFS01_MASK) | (u8RefVoltageCpy<<ADMUX_BIT_REFS0);
    }
    return enuErrorStatLoc;
}

void ADC_voidInterruptEnable(void)
{
    SET_BIT(ADCSRA, ADCSRA_BIT_ADIE);
}

void ADC_voidInterruptDisble(void)
{
    CLR_BIT(ADCSRA, ADCSRA_BIT_ADIE);
}

tenuErrorStatus ADC_enuSetCallBack(const pf pfAdcISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfAdcISRCpy == NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfAdcISRGlb = pfAdcISRCpy;
    }

    return enuErrorStatLoc;
}

void __vector_16 (void) __attribute__ ((signal,used)) ;
void __vector_16 (void){
	if(pfAdcISRGlb!=NULL_PTR)
	{
		pfAdcISRGlb();
	}
}
