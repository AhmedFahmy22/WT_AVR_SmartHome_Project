/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : APP                                                 */
/* SWC     : Screen Switch                                       */
/* Version : 1.0                                                 */
/* Date    : 22 Jan 2024                                         */
/*****************************************************************/

/***************Description***************/
/*
*  Write the template for every mode on LCD when switching between modes/displays
*/
/****************************************/

/****************Includes****************/
#include "../LIB/STD_Types.h"

#include "../MCAL/GIE/GIE_interface.h"

#include "../HAL/CLCD/CLCD_interface.h"

#include "APP.h"
#include "Screen_Switch.h"
/****************************************/

/******Static Functions Declarations******/
static void APP_voidStatusMainDisplay(void);

static void APP_voidTimerMainDisplay(void);
static void APP_voidTimerSubDisplay1(void);
static void APP_voidTimerSubDisplay2(void);
static void APP_voidTimerSubDisplay3(void);
static void APP_voidTimerSubDisplay4(void);

static void APP_voidFanMainDisplay(void);
static void APP_voidFanSubDisplay1(void);
static void APP_voidFanSubDisplay2(void);

static void APP_voidLedMainDisplay(void);
static void APP_voidLedSubDisplay1(void);
static void APP_voidLedSubDisplay2(void);

static void APP_voidServoMainDisplay(void);
static void APP_voidServoSubDisplay1(void);

static void APP_voidPinChangeMainDisplay(void);
static void APP_voidPinChangeSubDisplay1(void);
static void APP_voidPinChangeSubDisplay2(void);
static void APP_voidPinChangeSubDisplay3(void);
/****************************************/

/**********Functions Definitions**********/
void APP_voidScreenSwitch(tenuMainDisplay enuMainDisplayCpy, tenuSubDisplay enuSubDisplayCpy)
{
	if(enuSubDisplayCpy==APP_SUB_DISPLAY_NA)
	{
		enuSubDisplayCpy=APP_SUB_DISPLAY_MAIN;
	}
	if((enuMainDisplayCpy==enuCurrentMainDisplayGlb)&&(enuSubDisplayCpy==enuCurrentSubDisplayGlb))
	{
		/*if the screen is already switched do nothing*/
	}
	else if(enuSubDisplayCpy==APP_SUB_DISPLAY_MAIN) /*for main screen*/
	{
		CLCD_enuSendCommand(CLCD_COMM_CLEAR);
		CLCD_enuSendCommand(CLCD_COMM_CURSOR_OFF);
		switch(enuMainDisplayCpy)
		{
		case APP_DISPLAY_STATUS:
			APP_voidStatusMainDisplay();
			break;
		case APP_DISPLAY_TIMER:
			APP_voidTimerMainDisplay();
			break;
		case APP_DISPLAY_FAN:
			APP_voidFanMainDisplay();
			break;
		case APP_DISPLAY_LED:
			APP_voidLedMainDisplay();
			break;
		case APP_DISPLAY_SERVO:
			APP_voidServoMainDisplay();
			break;
		case APP_DISPLAY_PIN_CHANGE:
			APP_voidPinChangeMainDisplay();
			break;
		default:
			break;
		}
	}
	else /*for sub-screen*/
	{
		CLCD_enuSendCommand(CLCD_COMM_CLEAR);
		switch(enuMainDisplayCpy)
		{
		case APP_DISPLAY_TIMER:

			if(enuSubDisplayCpy==APP_SUB_DISPLAY_1) /*Timer Sub-screen 1 (Timer Set)*/
			{
				APP_voidTimerSubDisplay1();
			}
			else if(enuSubDisplayCpy==APP_SUB_DISPLAY_2) /*Timer Sub-screen 2 (Light)*/
			{
				APP_voidTimerSubDisplay2();
			}
			else if(enuSubDisplayCpy==APP_SUB_DISPLAY_3) /*Timer Sub-screen 3 (Fan)*/
			{
				APP_voidTimerSubDisplay3();
			}
			else if(enuSubDisplayCpy==APP_SUB_DISPLAY_4) /*Timer Sub-screen 4 (Buzzer)*/
			{
				APP_voidTimerSubDisplay4();
			}
			break;
		case APP_DISPLAY_FAN:
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_1) /*Motor/Fan Sub-screen 1 (On/Off/Sensor)*/
			{
				APP_voidFanSubDisplay1();
			}
			else if(enuSubDisplayCpy==APP_SUB_DISPLAY_2) /*Motor/Fan Sub-screen 2 (Sensor config)*/
			{
				APP_voidFanSubDisplay2();
			}
			break;
		case APP_DISPLAY_LED:
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_1) /*Led Sub-screen 1 (On/Off/Sensor)*/
			{
				APP_voidLedSubDisplay1();
			}
			else if(enuSubDisplayCpy==APP_SUB_DISPLAY_2) /*Led Sub-screen 2 (Sensor config)*/
			{
				APP_voidLedSubDisplay2();
			}
			break;
		case APP_DISPLAY_SERVO:
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_1) /*Servo Sub-screen 1 (Angle set)*/
			{
				APP_voidServoSubDisplay1();
			}
			break;
		case APP_DISPLAY_PIN_CHANGE:
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_1) /*PIN Sub-screen 1 (Current PIN)*/
			{
				APP_voidPinChangeSubDisplay1();
			}
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_2) /*PIN Sub-screen 2 (New PIN)*/
			{
				APP_voidPinChangeSubDisplay2();
			}
			if(enuSubDisplayCpy==APP_SUB_DISPLAY_3) /*PIN Sub-screen 3 (Wrong PIN)*/
			{
				APP_voidPinChangeSubDisplay3();
			}
			break;
		default:
			break;
		}
	}
	/*Update Current Display*/
	enuCurrentMainDisplayGlb = enuMainDisplayCpy;
	enuCurrentSubDisplayGlb = enuSubDisplayCpy;
}

static void APP_voidStatusMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[Status]");
	CLCD_enuGotoxy(1,1);
	CLCD_enuWriteString("T = ");
	CLCD_enuGotoxy(1,9);
	CLCD_enuWriteString("L = ");
	CLCD_enuWriteChar('%');
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(2);
}

static void APP_voidTimerMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[Timer]");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidTimerSubDisplay1(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Timer Setting:");
	CLCD_enuGotoxy(1,2);
	CLCD_enuWriteString("00:00");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidTimerSubDisplay2(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_OFF);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Timer->Light:");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidTimerSubDisplay3(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Light->Fan:");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidTimerSubDisplay4(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Timer->Buzzer:");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidFanMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[Fan]");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("State: ");
}

static void APP_voidFanSubDisplay1(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Fan Setting:");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidFanSubDisplay2(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Fan->ON");
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("if T > 20");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidLedMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[Light]");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("State: ");
}

static void APP_voidLedSubDisplay1(void)
{

	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Light Setting:");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);

}

static void APP_voidLedSubDisplay2(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Light->OFF");
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("if L > 20");
	CLCD_enuGotoxy(1,9);
	CLCD_enuWriteChar('%');
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
}

static void APP_voidServoMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[Servo]");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("Angle: ");
}

static void APP_voidServoSubDisplay1(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Servo Setting:");
	CLCD_enuGotoxy(1,1);
	CLCD_enuWriteString("000");
	CLCD_enuGotoxy(1,14);
	CLCD_enuWriteChar(0);
	CLCD_enuWriteChar(3);

}

static void APP_voidPinChangeMainDisplay(void)
{
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("[PIN-Change]");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(1);
	CLCD_enuWriteChar(3);
}

static void APP_voidPinChangeSubDisplay1(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Current PIN:");
	CLCD_enuGotoxy(1,1);
	CLCD_enuWriteString("0000");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(1);
	CLCD_enuWriteChar(3);
	CLCD_enuGotoxy(0,1);
}

static void APP_voidPinChangeSubDisplay2(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("New PIN:");
	CLCD_enuGotoxy(1,1);
	CLCD_enuWriteString("0000");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(1);
	CLCD_enuWriteChar(3);
	CLCD_enuGotoxy(0,1);
}

static void APP_voidPinChangeSubDisplay3(void)
{
	CLCD_enuSendCommand(CLCD_COMM_CURSOR_OFF);
	CLCD_enuGotoxy(0,0);
	CLCD_enuWriteString("Wrong Pin");
	CLCD_enuGotoxy(0,14);
	CLCD_enuWriteChar(3);
}
/****************************************/
