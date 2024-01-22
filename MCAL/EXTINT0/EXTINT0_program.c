/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT0                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/Bit_Math.h"

#include "EXTINT0_interface.h"
#include "EXTINT0_private.h"
#include "EXTINT0_config.h"

pf pfExtint0ISRGlb = NULL_PTR;

void EXTINT0_voidInit(void)
{
    #if EXTINT0_MODE == EXTINT0_MODE_LOW
	CLR_BIT(MCUCR,MCUCR_BIT_ISC00);
	CLR_BIT(MCUCR,MCUCR_BIT_ISC01);
    #elif EXTINT0_MODE == EXTINT0_MODE_ON_CHANGE
	SET_BIT(MCUCR,MCUCR_BIT_ISC00);
	CLR_BIT(MCUCR,MCUCR_BIT_ISC01);
    #elif EXTINT0_MODE == EXTINT0_MODE_FALLING
	CLR_BIT(MCUCR,MCUCR_BIT_ISC00);
	SET_BIT(MCUCR,MCUCR_BIT_ISC01);
    #elif EXTINT0_MODE == EXTINT0_MODE_RISING
	SET_BIT(MCUCR,MCUCR_BIT_ISC00);
	SET_BIT(MCUCR,MCUCR_BIT_ISC01);
    #endif
}

void EXTINT0_voidEnable(void)
{
    SET_BIT(GICR, INT0);
}

void EXTINT0_voidDisable(void)
{
    CLR_BIT(GICR, INT0);
}

tenuErrorStatus EXTINT0_enuSetMode(const uint8 u8ModeCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    switch(u8ModeCpy)
    {
        case EXTINT0_MODE_LOW:
        CLR_BIT(MCUCR,MCUCR_BIT_ISC00);
    	CLR_BIT(MCUCR,MCUCR_BIT_ISC01);
        break;
        case EXTINT0_MODE_ON_CHANGE:
        SET_BIT(MCUCR,MCUCR_BIT_ISC00);
    	CLR_BIT(MCUCR,MCUCR_BIT_ISC01);
        break;
        case EXTINT0_MODE_FALLING:
        CLR_BIT(MCUCR,MCUCR_BIT_ISC00);
    	SET_BIT(MCUCR,MCUCR_BIT_ISC01);
        break;
        case EXTINT0_MODE_RISING:
        SET_BIT(MCUCR,MCUCR_BIT_ISC00);
    	SET_BIT(MCUCR,MCUCR_BIT_ISC01);
        break;
        default:
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
        break;
    }
    return enuErrorStatLoc;
}

tenuErrorStatus EXTINT0_enuSetCallBack(const pf pfExtint0FunCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    if(pfExtint0FunCpy == NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfExtint0ISRGlb = pfExtint0FunCpy;
    }
    return enuErrorStatLoc;
}



void __vector_1 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_1 (void){
    if(pfExtint0ISRGlb!=NULL_PTR)
    {
        pfExtint0ISRGlb();
    }
}
