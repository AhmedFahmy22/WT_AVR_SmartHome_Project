/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : APP                                                 */
/* SWC     : Screen Update                                       */
/* Version : 1.0                                                 */
/* Date    : 22 Jan 2024                                         */
/*****************************************************************/

/**************Description**************/
/*
*  Update data on LCD, such as sensor data, user input, etc
*/
/***************************************/

/****************Includes****************/
#include "../LIB/STD_Types.h"
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/GIE/GIE_interface.h"

#include "../HAL/CLCD/CLCD_interface.h"

#include "APP.h"
#include "Screen_Update.h"
/****************************************/

/******Static Functions Declarations******/
static void APP_voidStatusMainDispUpdate(void);

static void APP_voidTimerSub1DispUpdate(void);
static void APP_voidTimerSub2DispUpdate(void);
static void APP_voidTimerSub3DispUpdate(void);
static void APP_voidTimerSub4DispUpdate(void);

static void APP_voidFanMainDispUpdate(void);
static void APP_voidFanSub1DispUpdate(void);
static void APP_voidFanSub2DispUpdate(void);

static void APP_voidLedMainDispUpdate(void);
static void APP_voidLedub1DispUpdate(void);
static void APP_voidLedub2DispUpdate(void);

static void APP_voidServoMainDispUpdate(void);
static void APP_voidServoSub1DispUpdate(void);

static void APP_voidPinSubDispUpdate(void);
/****************************************/

/**********Functions Definitions**********/
void APP_voidScreenUpdate(void)
{
	if((enuCurrentMainDisplayGlb==APP_DISPLAY_STATUS)&&(strFlagsGlb.u8UpdateFlag==0)) /*Status main display update*/
	{
		APP_voidStatusMainDispUpdate();
	}
	else if(enuCurrentMainDisplayGlb==APP_DISPLAY_TIMER) /*Timer update*/
	{
		switch(enuCurrentSubDisplayGlb)
		{
		case APP_SUB_DISPLAY_MAIN: /*for timer main display -> show remaining time*/
			if(strStatesGlb.u8TimerState==1)
			{
				CLCD_enuGotoxy(1,8);
				APP_voidDisplayTime(u16TimerGlb);
			}
			break;
		case APP_SUB_DISPLAY_1: /*for timer sub 1 (Set timer) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidTimerSub1DispUpdate();
			}
			break;
		case APP_SUB_DISPLAY_2: /*for timer sub 2 (Light) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidTimerSub2DispUpdate();
			}
			break;
		case APP_SUB_DISPLAY_3: /*for timer sub 3 (Fan) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidTimerSub3DispUpdate();
			}
			break;
		case APP_SUB_DISPLAY_4: /*for timer sub 2 (Buzzer) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidTimerSub4DispUpdate();
			}
			break;
		default:
			break;
		}
	}
	else if(enuCurrentMainDisplayGlb==APP_DISPLAY_FAN)
	{
		switch(enuCurrentSubDisplayGlb)
		{
		case APP_SUB_DISPLAY_MAIN: /*for Fan main display -> show fan state (On/Off)*/
			APP_voidFanMainDispUpdate();
			break;
		case APP_SUB_DISPLAY_1: /*for Fan sub 1 (Set configuration) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidFanSub1DispUpdate();
			}
			break;
		case APP_SUB_DISPLAY_2: /*for Fan sub 2 (Sensor) -> update user input (upper & lower limits)*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidFanSub2DispUpdate();
			}
			break;
		default:
			break;
		}
	}
	else if(enuCurrentMainDisplayGlb==APP_DISPLAY_LED)
	{
		switch(enuCurrentSubDisplayGlb)
		{
		case APP_SUB_DISPLAY_MAIN: /*for Led main display -> show fan state (On/Off)*/
			APP_voidLedMainDispUpdate();
			break;
		case APP_SUB_DISPLAY_1: /*for Led sub 1 (Set configuration) -> update user input*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidLedub1DispUpdate();
			}
			break;
		case APP_SUB_DISPLAY_2: /*for Led sub 2 (Sensor) -> update user input (upper & lower limits)*/
			if(strFlagsGlb.u8UpdateFlag==1)
			{
				APP_voidLedub2DispUpdate();
			}
			break;
		default:
			break;
		}
	}
	else if(enuCurrentMainDisplayGlb==APP_DISPLAY_SERVO)
	{
		if((enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_MAIN)&&(strFlagsGlb.u8UpdateFlag==0)) /*Servo main -> show current angle*/
		{
			APP_voidServoMainDispUpdate();
		}
		else if((enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1)&&(strFlagsGlb.u8UpdateFlag==1)) /*Servo sub1 -> update user input (angle)*/
		{
			APP_voidServoSub1DispUpdate();
		}
		else
		{

		}
	}
	else if(enuCurrentMainDisplayGlb==APP_DISPLAY_PIN_CHANGE)
	{
		if((enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1 || enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_2)&&(strFlagsGlb.u8UpdateFlag==1))
		{
			APP_voidPinSubDispUpdate();
		}
		else
		{

		}
	}
	else
	{

	}
}

static void APP_voidStatusMainDispUpdate(void)
{
	CLCD_enuGotoxy(1,5);
	CLCD_enuWriteNum(((uint32)u16Adc0ReadGlb*256/1023));
	CLCD_enuWriteChar(' ');
	CLCD_enuGotoxy(1,13);
	CLCD_enuWriteNum(((uint32)u16Adc1ReadGlb*256/1023));
	CLCD_enuWriteChar('%');
	CLCD_enuWriteChar(' ');
}

void APP_voidDisplayTime(uint16 u16TimeCpy)
{
	CLCD_enuWriteChar((u16TimeCpy/600) +'0');
	CLCD_enuWriteChar((u16TimeCpy/60)%10 +'0');
	CLCD_enuWriteChar(':');
	CLCD_enuWriteChar((u16TimeCpy%60)/10 +'0');
	CLCD_enuWriteChar((u16TimeCpy%60)%10 +'0');
}

static void APP_voidTimerSub1DispUpdate(void)
{
	uint8 u8LocationLoc = 0;

	if(u8InputNumGlb>=2) /*Seconds*/
	{
		u8LocationLoc = 3+u8InputNumGlb;
	}
	else /*Minutes*/
	{
		u8LocationLoc = 2+u8InputNumGlb;
	}
	CLCD_enuGotoxy(1,u8LocationLoc);
	CLCD_enuWriteNum(u8ChoiceGlb);
	CLCD_enuGotoxy(1,u8LocationLoc);
}

static void APP_voidTimerSub2DispUpdate(void)
{
	CLCD_enuGotoxy(1,0);
	switch(u8ChoiceGlb)
	{
	case 0:
		CLCD_enuWriteString("ON  ");
		break;
	case 1:
		CLCD_enuWriteString("OFF ");
		break;
	case 2:
		CLCD_enuWriteString("Skip");
		break;
	default:
		break;
	}
}

static void APP_voidTimerSub3DispUpdate(void)
{
	CLCD_enuGotoxy(1,0);
	switch(u8ChoiceGlb)
	{
	case 0:
		CLCD_enuWriteString("ON  ");
		break;
	case 1:
		CLCD_enuWriteString("OFF ");
		break;
	case 2:
		CLCD_enuWriteString("Skip");
		break;
	default:
		break;
	}
}

static void APP_voidTimerSub4DispUpdate(void)
{
	CLCD_enuGotoxy(1,0);
	switch(u8ChoiceGlb)
	{
	case 0:
		CLCD_enuWriteString("ON  ");
		break;
	case 1:
		CLCD_enuWriteString("OFF ");
		break;
	default:
		break;
	}
}

static void APP_voidFanMainDispUpdate(void)
{

	CLCD_enuGotoxy(1,7);
	if(strStatesGlb.u8FanState==0)
	{
		CLCD_enuWriteString("OFF");
	}
	else
	{
		CLCD_enuWriteString("ON ");
	}
}

static void APP_voidFanSub1DispUpdate(void)
{
	CLCD_enuGotoxy(1,0);
	switch(u8ChoiceGlb)
	{
	case 0:
		CLCD_enuWriteString("ON    ");
		break;
	case 1:
		CLCD_enuWriteString("OFF   ");
		break;
	case 2:
		CLCD_enuWriteString("Sensor");
		break;
	case 3:
		CLCD_enuWriteString("Cancel");
		break;
	default:
		break;
	}
}

static void APP_voidFanSub2DispUpdate(void)
{
	static uint8 u8EntryLoc = 0;
	uint8 u8LocationLoc = 0;
	if(u8InputNumGlb<2) /*1st limit*/
	{
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==0))
		{
			u8ChoiceGlb=2;
		}
		u8LocationLoc = 7+u8InputNumGlb;
	}
	else /*2nd limit*/
	{
		u8LocationLoc = 5+u8InputNumGlb;
	}

	if((u8InputNumGlb==2)&&(u8EntryLoc==0)) /*user entered 1st limit -> update display to get 2nd limit*/
	{
		u8EntryLoc=1;
		CLCD_enuGotoxy(0,5);
		CLCD_enuWriteString("OFF:");
		CLCD_enuGotoxy(1,5);
		CLCD_enuWriteString("< 10");
	}
	else if((u8InputNumGlb==3)&&(u8EntryLoc==1))
	{
		u8EntryLoc=0;
	}
	CLCD_enuGotoxy(1,u8LocationLoc);
	CLCD_enuWriteChar('0'+u8ChoiceGlb);
	CLCD_enuGotoxy(1,u8LocationLoc);
}

static void APP_voidLedMainDispUpdate(void)
{
	CLCD_enuGotoxy(1,7);
	if(strStatesGlb.u8LightState==0)
	{
		CLCD_enuWriteString("OFF");
	}
	else
	{
		CLCD_enuWriteString("ON ");
	}
}

static void APP_voidLedub1DispUpdate(void)
{
	CLCD_enuGotoxy(1,0);
	switch(u8ChoiceGlb)
	{
	case 0:
		CLCD_enuWriteString("ON    ");
		break;
	case 1:
		CLCD_enuWriteString("OFF   ");
		break;
	case 2:
		CLCD_enuWriteString("Sensor");
		break;
	case 3:
		CLCD_enuWriteString("Cancel");
		break;
	}
}

static void APP_voidLedub2DispUpdate(void)
{
	static uint8 u8EntryLoc = 0;
	uint8 u8LocatioLoc = 0;

	if(u8InputNumGlb<2) /*1st limit*/
	{
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==0))
		{
			u8ChoiceGlb=2;
		}
		u8LocatioLoc = 7+u8InputNumGlb;
	}
	else /*2nd limit*/
	{
		u8LocatioLoc = 5+u8InputNumGlb;
	}
	if((u8InputNumGlb==2)&&(u8EntryLoc==0)) /*user entered 1st limit -> update display to get 2nd limit*/
	{
		u8EntryLoc=1;
		CLCD_enuGotoxy(0,7);
		CLCD_enuWriteString("ON: ");
		CLCD_enuGotoxy(1,5);
		CLCD_enuWriteString("< 10");
	}
	else if((u8InputNumGlb==3)&&(u8EntryLoc==1))
	{
		u8EntryLoc=0;
	}
	CLCD_enuGotoxy(1,u8LocatioLoc);
	CLCD_enuWriteChar('0'+u8ChoiceGlb);
	CLCD_enuGotoxy(1,u8LocatioLoc);
}

static void APP_voidServoMainDispUpdate(void)
{
	CLCD_enuGotoxy(1,7);
	CLCD_enuWriteNum(strStatesGlb.u8ServoState);
	CLCD_enuWriteString("  ");
}

static void APP_voidServoSub1DispUpdate(void)
{
	CLCD_enuGotoxy(1,1+u8InputNumGlb);
	CLCD_enuWriteChar(u8ChoiceGlb+'0');
	CLCD_enuGotoxy(1,1+u8InputNumGlb);
}

static void APP_voidPinSubDispUpdate(void)
{
	CLCD_enuGotoxy(1,1+u8InputNumGlb);
	CLCD_enuWriteChar(u8ChoiceGlb+'0');
	CLCD_enuGotoxy(1,1+u8InputNumGlb);
}
/****************************************/
