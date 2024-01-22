/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : SERVO                                               */
/* Version : 1.0                                                 */
/* Date    : 14 Jan 2024                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"

#include "../../MCAL/TIMER1/TIMER1_interface.h"

#include "SERVO_interface.h"
#include "SERVO_private.h"
#include "SERVO_config.h"

void SERVO_voidInit(void)
{
    TIMER1_voidInit();
    TIMER1_voidSetICR1Value(SERVO_TIMER1_MAX);
    TIMER1_voidSetOCR1AValue(SERVO_N90);
}

tenuErrorStatus SERVO_enuGotoAngle(const uint8 u8AngleCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    uint32 u32TempLoc = 0;


    if(u8AngleCpy>180)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
    	u32TempLoc = ((SERVO_P90-SERVO_N90)*(uint32)u8AngleCpy/180)+SERVO_N90;
        TIMER1_voidSetOCR1AValue((uint16)u32TempLoc);
    }

    return enuErrorStatLoc;
}

tenuErrorStatus SERVO_enuGetAngle(uint8* const pu8AngleCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    uint32 u32TempLoc = 0;

    if(pu8AngleCpy==NULL_PTR)
    {
        enuErrorStatLoc=E_NOK_NULL_POINTER;
    }
    else
    {
    	u32TempLoc = TIMER1_u16GetOCR1AValue()-SERVO_N90;
    	u32TempLoc = (u32TempLoc * 180)/(SERVO_P90-SERVO_N90);
        *pu8AngleCpy = (uint8) u32TempLoc+1;
    }

    return enuErrorStatLoc;
}
