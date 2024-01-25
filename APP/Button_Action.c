/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : APP                                                 */
/* SWC     : Button Action                                       */
/* Version : 1.0                                                 */
/* Date    : 22 Jan 2024                                         */
/*****************************************************************/

/**************Description**************/
/*
 *  Contains the functions that take action according to user input
 */
/***************************************/

/****************Includes****************/
#include "../LIB/STD_Types.h"
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/I2C/I2C_interface.h"
#include "../HAL/EXTEEPROM/EXTEEPROM_interface.h"
#include "../HAL/CLCD/CLCD_interface.h"
#include "../HAL/SERVO/SERVO_interface.h"

#include "APP.h"
#include "Screen_Switch.h"
#include "Button_Action.h"
/****************************************/

/******Static Functions Declarations******/
static void APP_voidTimerSubUpdate(uint8 u8ButtonNumCpy);
static void APP_voidTimerSub1(uint8 u8ButtonNumCpy);
static void APP_voidTimerSub2(uint8 u8ButtonNumCpy);
static void APP_voidTimerSub3(uint8 u8ButtonNumCpy);
static void APP_voidTimerSub4(uint8 u8ButtonNumCpy);

static void APP_voidFanSubUpdate(uint8 u8ButtonNumCpy);
static void APP_voidFanSub1(uint8 u8ButtonNumCpy);
static void APP_voidFanSub2(uint8 u8ButtonNumCpy);

static void APP_voidLightSubUpdate(uint8 u8ButtonNumCpy);
static void APP_voidLightSub1(uint8 u8ButtonNumCpy);
static void APP_voidLightSub2(uint8 u8ButtonNumCpy);

static void APP_voidServoSubUpdate(uint8 u8ButtonNumCpy);
static void APP_voidServoSub1(uint8 u8ButtonNumCpy);

static void APP_voidPinChangeSubUpdate(uint8 u8ButtonNumCpy);
/****************************************/

/************Global Variables***********/
static tenuErrorStatus enuErrorStatGlb = E_OK;
/****************************************/

/**********Functions Definitions**********/
tenuErrorStatus APP_enuButtonAction(void)
{
	tenuSubDisplay enuCurrentSubDisplayLoc = enuCurrentSubDisplayGlb;
	if(strFlagsGlb.u8Button1Flag==1) /*UP Button Pressed*/
	{
		strFlagsGlb.u8Button1Flag=0;

		if(enuCurrentMainDisplayGlb==APP_DISPLAY_STATUS)/*UP button for status screen (go to last screen -> Pin Change)*/
		{
			enuErrorStatGlb |= APP_enuScreenSwitch(APP_DISPLAY_PIN_CHANGE,APP_SUB_DISPLAY_MAIN);
		}
		else
		{
			if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_MAIN)/*If current screen is a main screen*/
			{
				/*Go to previous main screen*/
				enuErrorStatGlb |= APP_enuScreenSwitch(enuCurrentMainDisplayGlb-1,APP_SUB_DISPLAY_MAIN);
			}
			else /*for sub screen -> run the corresponding sub-update function*/
			{
				switch(enuCurrentMainDisplayGlb)
				{
				case APP_DISPLAY_TIMER:
					APP_voidTimerSubUpdate(0);
					break;
				case APP_DISPLAY_FAN:
					APP_voidFanSubUpdate(0);
					break;
				case APP_DISPLAY_LED:
					APP_voidLightSubUpdate(0);
					break;
				case APP_DISPLAY_SERVO:
					APP_voidServoSubUpdate(0);
					break;
				case APP_DISPLAY_PIN_CHANGE:
					APP_voidPinChangeSubUpdate(0);
					break;
				default:
					break;
				}
			}
		}
	}
	else if(strFlagsGlb.u8Button2Flag==1) /*DOWN Button Pressed*/
	{
		strFlagsGlb.u8Button2Flag=0;
		if((enuCurrentMainDisplayGlb==APP_DISPLAY_PIN_CHANGE)&&(enuCurrentSubDisplayLoc==APP_SUB_DISPLAY_MAIN))/*Down button for PIN-Change screen (go to first screen -> Status)*/
		{
			enuErrorStatGlb |= APP_enuScreenSwitch(APP_DISPLAY_STATUS,APP_SUB_DISPLAY_MAIN);
		}
		else
		{
			if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_MAIN)/*If current screen is a main screen*/
			{
				/*Go to next main screen*/
				enuErrorStatGlb |= APP_enuScreenSwitch(enuCurrentMainDisplayGlb+1,APP_SUB_DISPLAY_MAIN);
			}
			else
			{
				switch(enuCurrentMainDisplayGlb)
				{
				case APP_DISPLAY_TIMER:
					APP_voidTimerSubUpdate(1);
					break;
				case APP_DISPLAY_FAN:
					APP_voidFanSubUpdate(1);
					break;
				case APP_DISPLAY_LED:
					APP_voidLightSubUpdate(1);
					break;
				case APP_DISPLAY_SERVO:
					APP_voidServoSubUpdate(1);
					break;
				case APP_DISPLAY_PIN_CHANGE:
					APP_voidPinChangeSubUpdate(1);
					break;
				default:
					break;
				}
			}
		}
	}
	else if(strFlagsGlb.u8Button3Flag==1) /*Enter Button pressed*/
	{
		strFlagsGlb.u8Button3Flag=0;
		if(enuCurrentMainDisplayGlb!=APP_DISPLAY_STATUS) /*if current display is not status*/
		{
			if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_MAIN)/*current display is main display -> go to next sub display*/
			{
				enuErrorStatGlb |= APP_enuScreenSwitch(enuCurrentMainDisplayGlb, enuCurrentSubDisplayLoc+1);
			}
			else
			{
				switch(enuCurrentMainDisplayGlb)
				{
				case APP_DISPLAY_TIMER:
					APP_voidTimerSubUpdate(2);
					break;
				case APP_DISPLAY_FAN:
					APP_voidFanSubUpdate(2);
					break;
				case APP_DISPLAY_LED:
					APP_voidLightSubUpdate(2);
					break;
				case APP_DISPLAY_SERVO:
					APP_voidServoSubUpdate(2);
					break;
				case APP_DISPLAY_PIN_CHANGE:
					APP_voidPinChangeSubUpdate(2);
					break;
				default:
					break;
				}

				if(enuCurrentSubDisplayLoc!=enuCurrentSubDisplayGlb) /*current display changed -> go to current display*/
				{
					enuCurrentSubDisplayLoc = enuCurrentSubDisplayGlb;
					enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_NA;
					enuErrorStatGlb |= APP_enuScreenSwitch(enuCurrentMainDisplayGlb, enuCurrentSubDisplayLoc);
				}
			}
		}
		else /*if current display is status display -> do nothing*/
		{

		}
	}
	else /*No button pressed -> no update*/
	{
		strFlagsGlb.u8UpdateFlag=0;
	}

	return enuErrorStatGlb;
}

static void APP_voidTimerSubUpdate(uint8 u8ButtonNumCpy)
{
	switch(enuCurrentSubDisplayGlb)
	{
	case APP_SUB_DISPLAY_1: /*Timer set*/
		APP_voidTimerSub1(u8ButtonNumCpy);
		break;
	case APP_SUB_DISPLAY_2: /*Timer -> Led*/
		APP_voidTimerSub2(u8ButtonNumCpy);
		break;
	case APP_SUB_DISPLAY_3: /*Timer -> Fan*/
		APP_voidTimerSub3(u8ButtonNumCpy);
		break;
	case APP_SUB_DISPLAY_4: /*Timer -> Buzzer*/
		APP_voidTimerSub4(u8ButtonNumCpy);
		break;
	default:
		break;
	}
}

static void APP_voidTimerSub1(uint8 u8ButtonNumCpy)
{
	static uint16 u16TimerTempLoc = 0;

	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button*/
		if(((u8InputNumGlb!=2)&&(u8ChoiceGlb==9))||((u8InputNumGlb==2)&&(u8ChoiceGlb==5)))
		{
			/*if choice = 9 or seconds 10's choice = 5 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*Down button*/
		if(u8ChoiceGlb==0) /*if choice = 0*/
		{
			if(u8InputNumGlb==2) /*seconds 10's choice -> got to 5*/
			{
				u8ChoiceGlb=5;
			}
			else
			{
				u8ChoiceGlb=9;
			}
		}
		else /*decrement choice*/
		{
			u8ChoiceGlb--;
		}
		break;
	case 2: /*Enter button*/
		if(u8InputNumGlb==0) /*minutes 10's choice*/
		{
			u16TimerTempLoc += u8ChoiceGlb*600;
			/*go to next input*/
			u8InputNumGlb++;
		}
		else if(u8InputNumGlb==1) /*minutes choice*/
		{
			u16TimerTempLoc += u8ChoiceGlb*60;
			/*go to next input*/
			u8InputNumGlb++;
		}
		else if(u8InputNumGlb==2) /*seconds 10's choice*/
		{
			u16TimerTempLoc += u8ChoiceGlb*10;
			/*go to next input*/
			u8InputNumGlb++;
		}
		else if(u8InputNumGlb==3) /*seconds choice*/
		{
			u16TimerTempLoc += u8ChoiceGlb;

			/*disable current timer*/
			strStatesGlb.u8TimerState=0;

			/*update timer value*/
			u16TimerGlb=u16TimerTempLoc;

			if(u16TimerGlb==0) /*if new timer value = 0 -> return to main display*/
			{
				enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
			}
			else /*if timer value != 0 -> go to next sub display (timer -> light)*/
			{
				enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_2;
			}
			u16TimerTempLoc=0;
			u8InputNumGlb=0;
		}
		else
		{
			/*wrong input number -> nothing*/
		}
		u8ChoiceGlb=0;
		break;
	default:
		break;
	}

}

static void APP_voidTimerSub2(uint8 u8ButtonNumCpy)
{
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if(u8ChoiceGlb==2) /*max number of choices = 2 -> go to choice 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*current choice = 0 -> go to last choice 2*/
		if(u8ChoiceGlb==0)
		{
			u8ChoiceGlb=2;
		}
		else
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8ChoiceGlb==0) /*choice 0 (On) -> turn led on after timer*/
		{
			strTimerOptionsGlb.u8LightAction=1;
		}
		else if(u8ChoiceGlb==1) /*choice 1 (Off) -> turn led off after timer*/
		{
			strTimerOptionsGlb.u8LightAction=2;
		}
		else if(u8ChoiceGlb==2) /*choice 2 (skip) -> timer doesn't affect led state*/
		{
			strTimerOptionsGlb.u8LightAction=0;
		}
		else
		{
			/*nothing*/
		}

		/*go to next sub display (timer -> fan)*/
		enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_3;

		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidTimerSub3(uint8 u8ButtonNumCpy)
{
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if(u8ChoiceGlb==2) /*max number of choices = 2 -> go to choice 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*current choice = 0 -> go to last choice 2*/
		if(u8ChoiceGlb==0)
		{
			u8ChoiceGlb=2;
		}
		else
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8ChoiceGlb==0) /*choice 0 (On) -> turn fan on after timer*/
		{
			strTimerOptionsGlb.u8FanAction=1;
		}
		else if(u8ChoiceGlb==1) /*choice 1 (Off) -> turn fan on after timer*/
		{
			strTimerOptionsGlb.u8FanAction=2;
		}
		else if(u8ChoiceGlb==2) /*choice 2 (skip) -> timer doesn't affect fan state*/
		{
			strTimerOptionsGlb.u8FanAction=0;
		}
		else
		{
			/*nothing*/
		}

		/*go to next sub display (timer -> Buzzer)*/
		enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_4;

		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidTimerSub4(uint8 u8ButtonNumCpy)
{
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if(u8ChoiceGlb==1) /*max number of choices = 1 -> go to choice 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*current choice = 0 -> go to last choice 1*/
		if(u8ChoiceGlb==0)
		{
			u8ChoiceGlb=1;
		}
		else
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8ChoiceGlb==0) /*choice 0 (On) -> turn buzzer on after timer*/
		{
			strTimerOptionsGlb.u8BuzzerAction=1;
		}
		else if(u8ChoiceGlb==1) /*choice 1 (Off) -> no buzzer after timer*/
		{
			strTimerOptionsGlb.u8BuzzerAction=2;
		}
		else
		{
			/*nothing*/
		}

		/*return to main display*/
		enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;

		/*Enable Timer*/
		strStatesGlb.u8TimerState=1;

		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidFanSubUpdate(uint8 u8ButtonNumCpy)
{
	if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1)
	{
		APP_voidFanSub1(u8ButtonNumCpy);
	}
	else if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_2)
	{
		APP_voidFanSub2(u8ButtonNumCpy);
	}
	else
	{
		/*nothing*/
	}

}

static void APP_voidFanSub1(uint8 u8ButtonNumCpy)
{
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if(u8ChoiceGlb==3) /*max number of choices = 3 -> go to choice 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*current choice = 0 -> go to last choice 3*/
		if(u8ChoiceGlb==0)
		{
			u8ChoiceGlb=3;
		}
		else
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8ChoiceGlb==0) /*choice 0 (On) -> turn fan on*/
		{
			APP_enuFanOn();

			/*Disable fan -> sensor by setting lower limit to 0*/
			strSensorOptionsGlb.u8TimpLower=0;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		else if(u8ChoiceGlb==1) /*choice 1 (Off) -> turn fan off*/
		{
			APP_enuFanOff();

			/*Disable fan -> sensor by setting upper limit to 0*/
			strSensorOptionsGlb.u8TimpUpper=0;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		else if(u8ChoiceGlb==2) /*choice 2 (sensor) -> go to (fan -> sensor) sub display*/
		{
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_2;
		}
		else /*choice 3 (cancel) -> return to main display*/
		{
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidFanSub2(uint8 u8ButtonNumCpy)
{
	static uint8 u8TempUpperLimitLoc = 0;
	static uint8 u8TempLowerLimitLoc = 0;

	/* upper limit minimum = 20
	 * upper limit maximum = 40
	 * lower limit minimum = 10
	 * lower limit maximum =  (upper limit - 5) */
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==4)) /*upper 10's number*/
		{
			/*upper limit 10's choice = 9 -> go to 2*/
			u8ChoiceGlb=2;
		}
		else if((u8InputNumGlb==1)&&(u8ChoiceGlb==9)) /*upper first number*/
		{
			/*upper limit choice = 9 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if((u8InputNumGlb==2)&&(u8ChoiceGlb==(u8TempUpperLimitLoc-5)/10)) /*lower 10's number*/
		{
			/*lower limit 10's choice = (upper-5) 10's -> go to 1*/
			u8ChoiceGlb=1;
		}
		else if(u8InputNumGlb==3) /*lower first number*/
		{
			if((u8TempLowerLimitLoc/10<(u8TempUpperLimitLoc-5)/10)&&u8ChoiceGlb==9)
			{
				/*(lower limit 10's choice < (upper-5) 10's) & (lower limit choice = 9) -> go to 0*/
				u8ChoiceGlb=0;
			}
			else if((u8TempLowerLimitLoc/10==(u8TempUpperLimitLoc-5)/10)&&(u8ChoiceGlb==(u8TempUpperLimitLoc-5)%10))
			{
				/*(lower limit 10's choice = (upper-5) 10's) & (lower limit choice = (upper - 5)) -> go to 0*/
				u8ChoiceGlb=0;
			}
			else /*increment*/
			{
				u8ChoiceGlb++;
			}
		}
		else /*increment*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*Down button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==2)) /*upper 10's number*/
		{
			/*upper limit 10's choice = 2 -> go to 4*/
			u8ChoiceGlb=4;
		}
		else if((u8InputNumGlb==1)&&(u8ChoiceGlb==0)) /*upper first number*/
		{
			/*upper limit choice = 0 -> go to 9*/
			u8ChoiceGlb=9;
		}
		else if((u8InputNumGlb==2)&&(u8ChoiceGlb==1)) /*lower 10's number*/
		{
			/*lower limit 10's choice = 1 -> go to (upper - 5) 10's*/
			u8ChoiceGlb=(u8TempUpperLimitLoc-5)/10;
		}
		else if(u8InputNumGlb==3) /*lower first number*/
		{
			if((u8TempLowerLimitLoc/10<(u8TempUpperLimitLoc-5)/10)&&u8ChoiceGlb==0)
			{
				/*(lower limit 10's choice < (upper-5) 10's) & (lower limit choice = 0) -> go to 9*/
				u8ChoiceGlb=9;
			}
			else if((u8TempLowerLimitLoc/10==(u8TempUpperLimitLoc-5)/10)&&(u8ChoiceGlb==0))
			{
				/*(lower limit 10's choice = (upper-5) 10's) & (lower limit choice = 0) -> go to (upper - 5)*/
				u8ChoiceGlb=(u8TempUpperLimitLoc-5)%10;
			}
			else /*decrement*/
			{
				u8ChoiceGlb--;
			}
		}
		else /*decrement*/
		{
			u8ChoiceGlb--;
		}
		break;
	case 2: /*Enter button pressed*/
		if(u8InputNumGlb==0) /*upper 10's number*/
		{
			u8TempUpperLimitLoc+=u8ChoiceGlb*10;
			u8InputNumGlb=1;
			u8ChoiceGlb=0;
		}
		else if(u8InputNumGlb==1) /*upper first number*/
		{
			u8TempUpperLimitLoc+=u8ChoiceGlb;
			u8InputNumGlb=2;
			u8ChoiceGlb=1;
		}
		else if(u8InputNumGlb==2) /*lower 10's number*/
		{
			u8TempLowerLimitLoc+=u8ChoiceGlb*10;
			u8InputNumGlb=3;
			u8ChoiceGlb=0;
		}
		else if(u8InputNumGlb==3)
		{
			u8TempLowerLimitLoc+=u8ChoiceGlb;

			/*update (temperature sensor) upper & lower limits*/
			strSensorOptionsGlb.u8TimpUpper=u8TempUpperLimitLoc;
			strSensorOptionsGlb.u8TimpLower=u8TempLowerLimitLoc;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;

			u8TempUpperLimitLoc=0;
			u8TempLowerLimitLoc=0;
			u8InputNumGlb=0;
			u8ChoiceGlb=0;
		}
		else
		{
			/*nothing*/
		}

		break;
	default:
		break;
	}
}

static void APP_voidLightSubUpdate(uint8 u8ButtonNumCpy)
{
	if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1)
	{
		APP_voidLightSub1(u8ButtonNumCpy); /*Led configurations*/
	}
	else if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_2)
	{
		APP_voidLightSub2(u8ButtonNumCpy); /*Led -> sensor*/
	}
	else
	{
		/*nothing*/
	}

}

static void APP_voidLightSub1(uint8 u8ButtonNumCpy)
{
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if(u8ChoiceGlb==3) /*max number of choices = 3 -> go to choice 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment choice*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*current choice = 0 -> go to last choice 3*/
		if(u8ChoiceGlb==0)
		{
			u8ChoiceGlb=3;
		}
		else
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8ChoiceGlb==0) /*choice 0 (On) -> turn led on*/
		{
			APP_enuLightOn();

			/*Disable light -> sensor by setting upper limit to 0*/
			strSensorOptionsGlb.u8LightUpper=0;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		else if(u8ChoiceGlb==1) /*choice 1 (Off) -> turn led off*/
		{
			APP_enuLightOff();

			/*Disable light -> sensor by setting lower limit to 0*/
			strSensorOptionsGlb.u8LightLower=0;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		else if(u8ChoiceGlb==2)  /*choice 2 (sensor) -> go to (light -> sensor) sub display*/
		{
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_2;
		}
		else /*choice 3 (cancel) -> return to main display*/
		{
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidLightSub2(uint8 u8ButtonNumCpy)
{
	static uint8 u8LightUpperLimitLoc = 0;
	static uint8 u8LightLowerLimitLoc = 0;

	/* upper limit minimum = 20
	 * upper limit maximum = 99
	 * lower limit minimum = 10
	 * lower limit maximum =  (upper limit - 10) */
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==9)) /*upper 10's number*/
		{
			/*upper limit 10's choice = 9 -> go to 2*/
			u8ChoiceGlb=2;
		}
		else if((u8InputNumGlb==1)&&(u8ChoiceGlb==9)) /*upper first number*/
		{
			/*upper limit choice = 9 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if((u8InputNumGlb==2)&&(u8ChoiceGlb==(u8LightUpperLimitLoc-10)/10)) /*lower 10's number*/
		{
			/*lower limit 10's choice = (upper-10) 10's -> go to 1*/
			u8ChoiceGlb=1;
		}
		else if(u8InputNumGlb==3) /*lower first number*/
		{
			if((u8LightLowerLimitLoc/10<(u8LightUpperLimitLoc-10)/10)&&u8ChoiceGlb==9)
			{
				/*(lower limit 10's choice < (upper-10) 10's) & (lower limit choice = 9) -> go to 0*/
				u8ChoiceGlb=0;
			}
			else if((u8LightLowerLimitLoc/10==(u8LightUpperLimitLoc-10)/10)&&(u8ChoiceGlb==u8LightUpperLimitLoc%10))
			{
				/*(lower limit 10's choice = (upper-10) 10's) & (lower limit choice = (upper - 10)) -> go to 0*/
				u8ChoiceGlb=0;
			}
			else /*increment*/
			{
				u8ChoiceGlb++;
			}
		}
		else /*increment*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*Down button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==2)) /*upper 10's number*/
		{
			/*upper limit 10's choice = 2 -> go to 9*/
			u8ChoiceGlb=9;
		}
		else if((u8InputNumGlb==1)&&(u8ChoiceGlb==0)) /*upper first number*/
		{
			/*upper limit choice = 0 -> go to 9*/
			u8ChoiceGlb=9;
		}
		else if((u8InputNumGlb==2)&&(u8ChoiceGlb==1)) /*lower 10's number*/
		{
			/*lower limit 10's choice = 1 -> go to (upper - 10) 10's*/
			u8ChoiceGlb=(u8LightUpperLimitLoc-10)/10;
		}
		else if(u8InputNumGlb==3) /*lower first number*/
		{
			if((u8LightLowerLimitLoc/10<(u8LightUpperLimitLoc-10)/10)&&u8ChoiceGlb==0)
			{
				/*(lower limit 10's choice < (upper-10) 10's) & (lower limit choice = 0) -> go to 9*/
				u8ChoiceGlb=9;
			}
			else if((u8LightLowerLimitLoc/10==(u8LightUpperLimitLoc-10)/10)&&(u8ChoiceGlb==0))
			{
				/*(lower limit 10's choice = (upper-10) 10's) & (lower limit choice = 0) -> go to (upper - 10)*/
				u8ChoiceGlb=(u8LightLowerLimitLoc-10)%10;
			}
			else /*decrement*/
			{
				u8ChoiceGlb--;
			}
		}
		else /*decrement*/
		{
			u8ChoiceGlb--;
		}
		break;
	case 2: /*Enter button pressed*/
		if(u8InputNumGlb==0) /*upper 10's number*/
		{
			u8LightUpperLimitLoc+=u8ChoiceGlb*10;
			u8InputNumGlb=1;
			u8ChoiceGlb=0;
		}
		else if(u8InputNumGlb==1) /*upper first number*/
		{
			u8LightUpperLimitLoc+=u8ChoiceGlb;
			u8InputNumGlb=2;
			u8ChoiceGlb=1;
		}
		else if(u8InputNumGlb==2) /*lower 10's number*/
		{
			u8LightLowerLimitLoc+=u8ChoiceGlb*10;
			u8InputNumGlb=3;
			u8ChoiceGlb=0;
		}
		else if(u8InputNumGlb==3) /*lower first number*/
		{
			u8LightLowerLimitLoc+=u8ChoiceGlb;

			/*update (light sensor) upper & lower limits*/
			strSensorOptionsGlb.u8LightUpper=u8LightUpperLimitLoc;
			strSensorOptionsGlb.u8LightLower=u8LightLowerLimitLoc;

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;

			u8LightUpperLimitLoc=0;
			u8LightLowerLimitLoc=0;
			u8InputNumGlb=0;
			u8ChoiceGlb=0;
		}
		else
		{
			/*nothing*/
		}
		break;
	default:
		break;
	}
}

static void APP_voidServoSubUpdate(uint8 u8ButtonNumCpy)
{
	if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1)
	{
		APP_voidServoSub1(u8ButtonNumCpy);
	}
	else
	{
		/*nothing*/
	}

}

static void APP_voidServoSub1(uint8 u8ButtonNumCpy)
{
	static uint8 u8AngleLoc = 0;

	/* Angle maximum = 180
	 * Angle minimum = 0 */
	switch(u8ButtonNumCpy)
	{
	case 0: /*Up button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==1)) /*100's number*/
		{
			/*if 100's choice = 1 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if((u8InputNumGlb==1)&&(u8AngleLoc/100==1)&&(u8ChoiceGlb==8)) /*10's number*/
		{
			/*if 100's number = 1 & 10's choice = 8 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if((u8InputNumGlb==2)&&(u8AngleLoc/10==18)) /*first number*/
		{
			/*if 100's number = 1 & 10's number = 8 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if(u8ChoiceGlb==9) /*if choice = 9 -> go to 0*/
		{
			u8ChoiceGlb=0;
		}
		else /*increment*/
		{
			u8ChoiceGlb++;
		}
		break;
	case 1: /*Down button pressed*/
		if((u8InputNumGlb==0)&&(u8ChoiceGlb==0)) /*100's number*/
		{
			/*if 100's choice = 0 -> go to 1*/
			u8ChoiceGlb=1;
		}
		else if((u8InputNumGlb==1)&&(u8AngleLoc/100==1)&&(u8ChoiceGlb==0)) /*10's number*/
		{
			/*if 100's number = 1 & 10's choice = 0 -> go to 8*/
			u8ChoiceGlb=8;
		}
		else if((u8InputNumGlb==2)&&(u8AngleLoc/10==18)) /*first number*/
		{
			/*if 100's number = 1 & 10's number = 8 -> go to 0*/
			u8ChoiceGlb=0;
		}
		else if(u8ChoiceGlb==0) /*if choice = 0 -> go to 9*/
		{
			u8ChoiceGlb=9;
		}
		else /*decrement*/
		{
			u8ChoiceGlb--;
		}
		break;
	case 2:
		if(u8InputNumGlb==0) /*100's number*/
		{
			u8AngleLoc+=u8ChoiceGlb*100;
			u8InputNumGlb=1;
		}
		else if(u8InputNumGlb==1) /*10's number*/
		{
			u8AngleLoc+=u8ChoiceGlb*10;
			u8InputNumGlb=2;
		}
		else if(u8InputNumGlb==2) /*first number*/
		{
			u8AngleLoc+=u8ChoiceGlb;

			/*Update current angle*/
			strStatesGlb.u8ServoState = u8AngleLoc;
			enuErrorStatGlb |= SERVO_enuGotoAngle(strStatesGlb.u8ServoState);

			/*return to main display*/
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;

			u8AngleLoc=0;
			u8InputNumGlb=0;
		}
		else
		{
			/*nothing*/
		}
		u8ChoiceGlb=0;
		break;
	default:
		break;
	}
}

static void APP_voidPinChangeSubUpdate(uint8 u8ButtonNumCpy)
{
	static uint16 u16PinTempLoc = 0;

	if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_3)
	{
		/*Wrong PIN & Enter button -> return to main*/
		if(u8ButtonNumCpy==2)
		{
			enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
		}
		else
		{

		}
	}
	else /*sub1(Enter current PIN) & sub2(Enter new PIN)*/
	{
		switch(u8ButtonNumCpy)
		{
		case 0: /*Up button pressed*/
			if(u8ChoiceGlb==9)
			{
				u8ChoiceGlb=0;
			}
			else
			{
				u8ChoiceGlb++;
			}
			break;
		case 1: /*Down button pressed*/
			if(u8ChoiceGlb==0)
			{
				u8ChoiceGlb=9;
			}
			else
			{
				u8ChoiceGlb--;
			}
			break;
		case 2: /*Enter button pressed*/
			if(u8InputNumGlb==0) /*1st digit*/
			{
				u16PinTempLoc += u8ChoiceGlb*1000;
				u8InputNumGlb=1;
			}
			else if(u8InputNumGlb==1) /*2nd digit*/
			{
				u16PinTempLoc += u8ChoiceGlb*100;
				u8InputNumGlb=2;
			}
			else if(u8InputNumGlb==2) /*3rd digit*/
			{
				u16PinTempLoc += u8ChoiceGlb*10;
				u8InputNumGlb=3;
			}
			else if(u8InputNumGlb==3) /*4th digit*/
			{
				u16PinTempLoc += u8ChoiceGlb;

				if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_1)/*current PIN check*/
				{
					if(u16PinTempLoc==u16CurrentPinGlb) /*Correct PIN*/
					{
						/*go to sub2 (Enter new PIN)*/
						enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_2;
					}
					else
					{
						/*go to sub3 (Wrong PIN)*/
						enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_3;
					}
				}
				else if(enuCurrentSubDisplayGlb==APP_SUB_DISPLAY_2)/*new PIN*/
				{
					/*update PIN*/
					u16CurrentPinGlb=u16PinTempLoc;

					/*send to EEPROM*/
					enuErrorStatGlb |= EXTEEPROM_enuWrite(0x51,(uint8)u16CurrentPinGlb);
					enuErrorStatGlb |= EXTEEPROM_enuWrite(0x52,(uint8)(u16CurrentPinGlb>>8));

					/*return to main display*/
					enuCurrentSubDisplayGlb = APP_SUB_DISPLAY_MAIN;
				}
				u16PinTempLoc=0;
				u8InputNumGlb=0;
			}
			else
			{
				/*nothing*/
			}
			u8ChoiceGlb=0;
			break;
		default:
			break;
		}

	}

}


tenuErrorStatus APP_enuLightOn(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	strStatesGlb.u8LightState=1;
	enuErrorStatLoc |= DIO_enuWritePinValue(DIO_PORTA, DIO_PIN2, DIO_HIGH);

	return enuErrorStatLoc;
}

tenuErrorStatus APP_enuLightOff(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	strStatesGlb.u8LightState=0;
	enuErrorStatLoc |= DIO_enuWritePinValue(DIO_PORTA, DIO_PIN2, DIO_LOW);

	return enuErrorStatLoc;
}

tenuErrorStatus APP_enuFanOn(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	strStatesGlb.u8FanState=1;
	enuErrorStatLoc |= DIO_enuWritePinValue(DIO_PORTA, DIO_PIN3, DIO_HIGH);

	return enuErrorStatLoc;
}

tenuErrorStatus APP_enuFanOff(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	strStatesGlb.u8FanState=0;
	enuErrorStatLoc |= DIO_enuWritePinValue(DIO_PORTA, DIO_PIN3, DIO_LOW);

	return enuErrorStatLoc;
}

void APP_voidBuzzerOn(void)
{
	strStatesGlb.u8BuzzerState = 1;
}

void APP_voidBuzzerOff(void)
{
	strStatesGlb.u8BuzzerState = 0;
}

tenuErrorStatus APP_enuBuzzerToggle(void)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;

	enuErrorStatLoc |= DIO_enuTogglePinValue(DIO_PORTA, DIO_PIN4);

	return enuErrorStatLoc;
}
/****************************************/
