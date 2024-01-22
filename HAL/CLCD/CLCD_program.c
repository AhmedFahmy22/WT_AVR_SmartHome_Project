/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : CLCD                                                */
/* Version : 1.0                                                 */
/* Date    : 11 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "../../MCAL/DIO/DIO_interface.h"

#include "../../MCAL/TIMER0/TIMER0_interface.h"

#include "CLCD_interface.h"
#include "CLCD_private.h"
#include "CLCD_config.h"
#include "CLCD_extra.h"

/*Static helper functions*/
static tenuErrorStatus CLCD_enuWriteData_Helper(const uint8 u8DataCpy);

/*Function Implementation*/
tenuErrorStatus CLCD_enuInit(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;
	uint8 u8CommLoc, u8CntrLoc1, u8CntrLoc2;

#if CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_8BIT
	/*Write Low on Enable Pin*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
	/*8-Bit initialization*/
	u8CommLoc = 0b00110000 | (CLCD_LINES_NUM<<3) | (CLCD_CHAR_FONT<<2);
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
	TIMER0_voidDelay(1);
	u8CommLoc = 0b00001100 | (CLCD_CURSOR_DISPLAY<<1) | (CLCD_CURSOR_BLINK);
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=CLCD_enuSendCommand(CLCD_COMM_CLEAR);
	TIMER0_voidDelay(1);
	u8CommLoc = 0b00000100 | (CLCD_CURSOR_MOVE<<1) | (CLCD_DISPLAY_SHIFT);
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
	TIMER0_voidDelay(1);
#elif  CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_4BIT
	/*Write Low on Enable Pin*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
	/*4-Bit initialization*/
	u8CommLoc = 0b00100010;
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
	u8CommLoc = 0 | (CLCD_LINES_NUM<<3) | (CLCD_CHAR_FONT<<2);
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8CommLoc);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
	u8CommLoc = 0b00001100 | (CLCD_CURSOR_DISPLAY<<1) | (CLCD_CURSOR_BLINK);
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
	enuErrorStatLoc|=CLCD_enuSendCommand(CLCD_COMM_CLEAR);
	u8CommLoc = 0b00000100 | (CLCD_CURSOR_MOVE<<1) | (CLCD_DISPLAY_SHIFT);
	enuErrorStatLoc|=CLCD_enuSendCommand(u8CommLoc);
#endif
    /*Load Custom Characters*/
    enuErrorStatLoc|=CLCD_enuSendCommand(0x40);
    TIMER0_voidDelay(1);
    for(u8CntrLoc1=0;u8CntrLoc1<CLCD_CUSTOM_CHAR_NUM;u8CntrLoc1++)
    {
        for(u8CntrLoc2=0;u8CntrLoc2<8;u8CntrLoc2++)
        {
            enuErrorStatLoc|=CLCD_enuWriteChar(au8CharGlb[u8CntrLoc1][u8CntrLoc2]);
            TIMER0_voidDelay(1);
        }
    }
    enuErrorStatLoc|=CLCD_enuSendCommand(0x80);
	return enuErrorStatLoc;
}

tenuErrorStatus CLCD_enuSendCommand(const uint8 u8CmdCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	/*Rs: Low (Command)*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_RSPIN, DIO_LOW);

	/*Rw: Low (Write)*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_RWPIN, DIO_LOW);

	/*Write Low on Enable Pin*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);

#if CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_8BIT
	/*Write Command on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8CmdCpy);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
#elif CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_4BIT
	/*Write Command(High Nibble) on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8CmdCpy>>4);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
	/*Write Command(low Nibble) on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8CmdCpy);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
#endif
	return enuErrorStatLoc;
}

tenuErrorStatus CLCD_enuWriteChar(const uint8 u8DataCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	/*Rs: High (Data)*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_RSPIN, DIO_HIGH);

	/*Rw: Low (Write)*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_RWPIN, DIO_LOW);

	/*Write Low on Enable Pin*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);

#if CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_8BIT
	/*Write Data on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8DataCpy);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
#elif CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_4BIT
	/*Write Data(High Nibble) on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8DataCpy>>4);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
	/*Write Data(Low Nibble) on Data Port*/
	enuErrorStatLoc|=CLCD_enuWriteData_Helper(u8DataCpy);
	/*Enable Sequence*/
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_HIGH);
	TIMER0_voidDelay(1);
	enuErrorStatLoc|=DIO_enuWritePinValue(CLCD_CONTROLPORT, CLCD_EPIN, DIO_LOW);
#endif
	return enuErrorStatLoc;
}

tenuErrorStatus CLCD_enuWriteString (const uint8* const pu8StringCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;
	uint8 u8CntrLoc=0;
	if(pu8StringCpy==NULL_PTR)
	{
		enuErrorStatLoc=E_NOK_NULL_POINTER;
	}
	else
	{
		while(*(pu8StringCpy+u8CntrLoc)!='\0')
		{
			enuErrorStatLoc|=CLCD_enuWriteChar(*(pu8StringCpy+u8CntrLoc));
			u8CntrLoc++;
		}
	}

	return enuErrorStatLoc;
}

tenuErrorStatus CLCD_enuGotoxy (const uint8 u8LineNumCpy, const uint8 u8ColumnCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;
	if((u8ColumnCpy>=0)&&(u8ColumnCpy<=15))
	{
		switch(u8LineNumCpy)
		{
		case 0:
			enuErrorStatLoc|=CLCD_enuSendCommand(u8ColumnCpy+CLCD_LINE_1_START);
			break;
		case 1:
			enuErrorStatLoc|=CLCD_enuSendCommand(u8ColumnCpy+CLCD_LINE_2_START);
			break;
		default:
			enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
			break;
		}
	}
	else
	{
		enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
	}
	return enuErrorStatLoc;
}

tenuErrorStatus CLCD_enuWriteNum(const sint32 s32DataCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;
	sint32 s32DataTempLoc = s32DataCpy;
	sint32 s32DataInvertLoc = 0;
	uint8 u8DataLengthLoc = 0;
	uint8 u8CntrLoc;

	do{
		u8DataLengthLoc++;
		s32DataInvertLoc += (s32DataTempLoc%10);
		s32DataTempLoc /= 10;
		s32DataInvertLoc*=10;
	}while(s32DataTempLoc!=0);
	s32DataInvertLoc/=10;
	if(s32DataCpy<0)
	{
		enuErrorStatLoc|=CLCD_enuWriteChar('-');
		if(s32DataInvertLoc<0)
		{
			s32DataInvertLoc *= -1;
		}
	}

	for(u8CntrLoc=0; u8CntrLoc<u8DataLengthLoc; u8CntrLoc++)
	{
		enuErrorStatLoc |= CLCD_enuWriteChar((s32DataInvertLoc%10)+'0');
		s32DataInvertLoc/=10;
	}

	return enuErrorStatLoc;
}

static tenuErrorStatus CLCD_enuWriteData_Helper(const uint8 u8DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    #if CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_8BIT
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D0_PORT, CLCD_D0_PIN, GET_BIT(u8DataCpy,0));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D1_PORT, CLCD_D1_PIN, GET_BIT(u8DataCpy,1));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D2_PORT, CLCD_D2_PIN, GET_BIT(u8DataCpy,2));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D3_PORT, CLCD_D3_PIN, GET_BIT(u8DataCpy,3));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D4_PORT, CLCD_D4_PIN, GET_BIT(u8DataCpy,4));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D5_PORT, CLCD_D5_PIN, GET_BIT(u8DataCpy,5));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D6_PORT, CLCD_D6_PIN, GET_BIT(u8DataCpy,6));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D7_PORT, CLCD_D7_PIN, GET_BIT(u8DataCpy,7));
    #elif CLCD_DATA_LENGTH == CLCD_DATA_LENGTH_4BIT
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D4_PORT, CLCD_D4_PIN, GET_BIT(u8DataCpy,0));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D5_PORT, CLCD_D5_PIN, GET_BIT(u8DataCpy,1));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D6_PORT, CLCD_D6_PIN, GET_BIT(u8DataCpy,2));
    enuErrorStatLoc |= DIO_enuWritePinValue(CLCD_D7_PORT, CLCD_D7_PIN, GET_BIT(u8DataCpy,3));
    #endif

    return enuErrorStatLoc;
}
