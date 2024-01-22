/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : DIO                                                 */
/* Version : 1.0                                                 */
/* Date    : 09 Dec 2023                                         */
/*****************************************************************/

#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"

void DIO_voidInit(void)
{
    DDRA = CONC(DIO_PIN_DIR_A7,DIO_PIN_DIR_A6,DIO_PIN_DIR_A5,DIO_PIN_DIR_A4,DIO_PIN_DIR_A3,DIO_PIN_DIR_A2,DIO_PIN_DIR_A1,DIO_PIN_DIR_A0);
    DDRB = CONC(DIO_PIN_DIR_B7,DIO_PIN_DIR_B6,DIO_PIN_DIR_B5,DIO_PIN_DIR_B4,DIO_PIN_DIR_B3,DIO_PIN_DIR_B2,DIO_PIN_DIR_B1,DIO_PIN_DIR_B0);
    DDRC = CONC(DIO_PIN_DIR_C7,DIO_PIN_DIR_C6,DIO_PIN_DIR_C5,DIO_PIN_DIR_C4,DIO_PIN_DIR_C3,DIO_PIN_DIR_C2,DIO_PIN_DIR_C1,DIO_PIN_DIR_C0);
    DDRD = CONC(DIO_PIN_DIR_D7,DIO_PIN_DIR_D6,DIO_PIN_DIR_D5,DIO_PIN_DIR_D4,DIO_PIN_DIR_D3,DIO_PIN_DIR_D2,DIO_PIN_DIR_D1,DIO_PIN_DIR_D0);
}

tenuErrorStatus DIO_enuWritePinValue(const uint8 u8PortCpy, const uint8 u8PinCpy, const uint8 u8ValueCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            if(u8ValueCpy==DIO_LOW)
            {
                CLR_BIT(PORTA, u8PinCpy);
            }
            else if(u8ValueCpy==DIO_HIGH)
            {
                SET_BIT(PORTA, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTB:
            if(u8ValueCpy==DIO_LOW)
            {
                CLR_BIT(PORTB, u8PinCpy);
            }
            else if(u8ValueCpy==DIO_HIGH)
            {
                SET_BIT(PORTB, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTC:
            if(u8ValueCpy==DIO_LOW)
            {
                CLR_BIT(PORTC, u8PinCpy);
            }
            else if(u8ValueCpy==DIO_HIGH)
            {
                SET_BIT(PORTC, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTD:
            if(u8ValueCpy==DIO_LOW)
            {
                CLR_BIT(PORTD, u8PinCpy);
            }
            else if(u8ValueCpy==DIO_HIGH)
            {
                SET_BIT(PORTD, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuReadPinValue(const uint8 u8PortCpy, const uint8 u8PinCpy, uint8* const pu8ValueCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else if(pu8ValueCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            *pu8ValueCpy = GET_BIT(PINA,u8PinCpy);
            break;
            case DIO_PORTB:
            *pu8ValueCpy = GET_BIT(PINB,u8PinCpy);
            break;
            case DIO_PORTC:
            *pu8ValueCpy = GET_BIT(PINC,u8PinCpy);
            break;
            case DIO_PORTD:
            *pu8ValueCpy = GET_BIT(PIND,u8PinCpy);
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuTogglePinValue(const uint8 u8PortCpy, const uint8 u8PinCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            TGL_BIT(PORTA,u8PinCpy);
            break;
            case DIO_PORTB:
            TGL_BIT(PORTB,u8PinCpy);
            break;
            case DIO_PORTC:
            TGL_BIT(PORTC,u8PinCpy);
            break;
            case DIO_PORTD:
            TGL_BIT(PORTD,u8PinCpy);
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuWritePortValue(const uint8 u8PortCpy, const uint8 u8ValueCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    switch(u8PortCpy)
    {
        case DIO_PORTA:
        PORTA = u8ValueCpy;
        break;
        case DIO_PORTB:
        PORTB = u8ValueCpy;
        break;
        case DIO_PORTC:
        PORTC = u8ValueCpy;
        break;
        case DIO_PORTD:
        PORTD = u8ValueCpy;
        break;
        default:
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
        break;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuReadPortValue(const uint8 u8PortCpy, uint8* const pu8ValueCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pu8ValueCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            *pu8ValueCpy = PINA;
            break;
            case DIO_PORTB:
            *pu8ValueCpy = PINB;
            break;
            case DIO_PORTC:
            *pu8ValueCpy = PINC;
            break;
            case DIO_PORTD:
            *pu8ValueCpy = PIND;
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuTogglePortValue(const uint8 u8PortCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    switch(u8PortCpy)
    {
        case DIO_PORTA:
        PORTA = ~PORTA;
        break;
        case DIO_PORTB:
        PORTB = ~PORTB;
        break;
        case DIO_PORTC:
        PORTC = ~PORTC;
        break;
        case DIO_PORTD:
        PORTD = ~PORTD;
        break;
        default:
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
        break;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuSetPinDirection(const uint8 u8PortCpy, const uint8 u8PinCpy, const uint8 u8DirCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            if(u8DirCpy==DIO_INPUT)
            {
                CLR_BIT(DDRA, u8PinCpy);
            }
            else if(u8DirCpy==DIO_OUTPUT)
            {
                SET_BIT(DDRA, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTB:
            if(u8DirCpy==DIO_INPUT)
            {
                CLR_BIT(DDRB, u8PinCpy);
            }
            else if(u8DirCpy==DIO_OUTPUT)
            {
                SET_BIT(DDRB, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTC:
            if(u8DirCpy==DIO_INPUT)
            {
                CLR_BIT(DDRC, u8PinCpy);
            }
            else if(u8DirCpy==DIO_OUTPUT)
            {
                SET_BIT(DDRC, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            case DIO_PORTD:
            if(u8DirCpy==DIO_INPUT)
            {
                CLR_BIT(DDRD, u8PinCpy);
            }
            else if(u8DirCpy==DIO_OUTPUT)
            {
                SET_BIT(DDRD, u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            }
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuGetPinDirection(const uint8 u8PortCpy, const uint8 u8PinCpy, uint8* const pu8DirCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else if(pu8DirCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            *pu8DirCpy = GET_BIT(DDRA,u8PinCpy);
            break;
            case DIO_PORTB:
            *pu8DirCpy = GET_BIT(DDRB,u8PinCpy);
            break;
            case DIO_PORTC:
            *pu8DirCpy = GET_BIT(DDRC,u8PinCpy);
            break;
            case DIO_PORTD:
            *pu8DirCpy = GET_BIT(DDRD,u8PinCpy);
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuSetPortDirection(const uint8 u8PortCpy, const uint8 u8DirCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    switch(u8PortCpy)
    {
        case DIO_PORTA:
        DDRA = u8DirCpy;
        break;
        case DIO_PORTB:
        DDRB = u8DirCpy;
        break;
        case DIO_PORTC:
        DDRC = u8DirCpy;
        break;
        case DIO_PORTD:
        DDRD = u8DirCpy;
        break;
        default:
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
        break;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuGetPortDirection(const uint8 u8PortCpy, uint8* const pu8DirCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pu8DirCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            *pu8DirCpy = DDRA;
            break;
            case DIO_PORTB:
            *pu8DirCpy = DDRB;
            break;
            case DIO_PORTC:
            *pu8DirCpy = DDRC;
            break;
            case DIO_PORTD:
            *pu8DirCpy = DDRD;
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuPullUpEnable(const uint8 u8PortCpy, const uint8 u8PinCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            if(GET_BIT(DDRA,u8PinCpy)==DIO_INPUT)
            {
                SET_BIT(PORTA,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTB:
            if(GET_BIT(DDRB,u8PinCpy)==DIO_INPUT)
            {
                SET_BIT(PORTB,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTC:
            if(GET_BIT(DDRC,u8PinCpy)==DIO_INPUT)
            {
                SET_BIT(PORTC,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTD:
            if(GET_BIT(DDRD,u8PinCpy)==DIO_INPUT)
            {
                SET_BIT(PORTD,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}

tenuErrorStatus DIO_enuPullUpDisable(const uint8 u8PortCpy, const uint8 u8PinCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if((u8PinCpy<0)||(u8PinCpy>7))
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        switch(u8PortCpy)
        {
            case DIO_PORTA:
            if(GET_BIT(DDRA,u8PinCpy)==DIO_INPUT)
            {
                CLR_BIT(PORTA,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTB:
            if(GET_BIT(DDRB,u8PinCpy)==DIO_INPUT)
            {
                CLR_BIT(PORTB,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTC:
            if(GET_BIT(DDRC,u8PinCpy)==DIO_INPUT)
            {
                CLR_BIT(PORTC,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            case DIO_PORTD:
            if(GET_BIT(DDRD,u8PinCpy)==DIO_INPUT)
            {
                CLR_BIT(PORTD,u8PinCpy);
            }
            else
            {
                enuErrorStatLoc = E_NOK;
            }
            break;
            default:
            enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
            break;
        }
    }

    return enuErrorStatLoc;
}
