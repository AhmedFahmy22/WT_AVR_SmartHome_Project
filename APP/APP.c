/*
 * APP.c
 *
 *  Created on: Jan 9, 2024
 *      Author: Ahmed
 */

/****************Includes****************/
#include "../LIB/STD_Types.h"

#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/USART/USART_interface.h"
#include "../MCAL/I2C/I2C_interface.h"
#include "../MCAL/TIMER0/TIMER0_interface.h"
#include "../MCAL/EXTINT0/EXTINT0_interface.h"
#include "../MCAL/EXTINT1/EXTINT1_interface.h"
#include "../MCAL/EXTINT2/EXTINT2_interface.h"
#include "../MCAL/GIE/GIE_interface.h"

#include "../HAL/CLCD/CLCD_interface.h"
#include "../HAL/SERVO/SERVO_interface.h"
#include "../HAL/EXTEEPROM/EXTEEPROM_interface.h"

#include "APP.h"
#include "Screen_Switch.h"
#include "Screen_Update.h"
#include "Button_Action.h"
/****************************************/

/******Static Functions Declarations******/
static void APP_voidADC_ISR(void);
static void APP_voidUSART_ISR(void);
static void APP_voidTimer0Cmp_ISR(void);
static void APP_voidInt0_ISR(void);
static void APP_voidInt1_ISR(void);
static void APP_voidInt2_ISR(void);
static void APP_voidPinCheck(const uint16 u16PinCodeCpy);
static void APP_voidTimer0Reset(void);
/****************************************/

/************Global Variables***********/
tstrFlags  strFlagsGlb;
tstrStates strStatesGlb;
tstrTimerOptions strTimerOptionsGlb;
tstrSensorOptions strSensorOptionsGlb;

uint16 u16Adc0ReadGlb;
uint16 u16Adc1ReadGlb;
uint16 u16TimerGlb;
tenuMainDisplay enuCurrentMainDisplayGlb = APP_DISPLAY_NA;
tenuSubDisplay enuCurrentSubDisplayGlb = APP_DISPLAY_NA;
uint8 u8ChoiceGlb = 0;
uint8 u8InputNumGlb = 0;
uint8 u8CurrentTempGlb;
uint8 u8CurrentLightGlb;
uint16 u16CurrentPinGlb = 1234;
/****************************************/

/**********Functions Definitions**********/
void APP_voidInit(void)
{
	uint8 u8PinStoreCheckLoc = 0xFF;

	/*DIO*/
	DIO_voidInit();
	DIO_enuPullUpEnable(DIO_PORTD, DIO_PIN2);
	DIO_enuPullUpEnable(DIO_PORTD, DIO_PIN3);
	DIO_enuPullUpEnable(DIO_PORTB, DIO_PIN2);

	/*EXEEPROM*/
	EXTEEPROM_voidInit();

	/*ADC*/
	ADC_voidInit();
	ADC_enuSetCallBack(APP_voidADC_ISR);

	/*UART*/
	USART_voidInit();
	USART_enuReceiveSetCallBack(APP_voidUSART_ISR);

	/*Timer0*/
	TIMER0_voidInit();
	TIMER0_voidSetCmpMatchValue(250);
	TIMER0_enuSetCallBackCmpMatch(APP_voidTimer0Cmp_ISR);
	TIMER0_voidIntCmpMatchEnable();

	/*External interrupts (Buttons)*/
	EXTINT0_voidInit();
	EXTINT0_enuSetCallBack(APP_voidInt0_ISR);
	EXTINT1_voidInit();
	EXTINT1_enuSetCallBack(APP_voidInt1_ISR);
	EXTINT2_voidInit();
	EXTINT2_enuSetCallBack(APP_voidInt2_ISR);

	/*Servo motor*/
	SERVO_voidInit();

	/*Enable interrupts*/
	ADC_voidInterruptEnable();
	EXTINT0_voidEnable();
	EXTINT1_voidEnable();
	EXTINT2_voidEnable();

	GIE_voidEnable();

	/*LCD*/
	CLCD_enuInit();

	/*Write / Read PIN from EEPROM*/
	EXTEEPROM_enuRead(0x50,&u8PinStoreCheckLoc);
	if(u8PinStoreCheckLoc==0xFF) /*If location == 0xFF, write default PIN : 1234*/
	{
		EXTEEPROM_enuWrite(0x50,0);
		EXTEEPROM_enuWrite(0x51,(uint8)u16CurrentPinGlb);
		EXTEEPROM_enuWrite(0x52,(uint8)(u16CurrentPinGlb>>8));
	}
	else /*Get stored PIN*/
	{
		EXTEEPROM_enuRead(0x51,(uint8*)(&u16CurrentPinGlb));
		EXTEEPROM_enuRead(0x52,(uint8*)(&u16CurrentPinGlb)+1);
	}

	/*Get and check PIN from user*/
	APP_voidPinCheck(u16CurrentPinGlb);

	/*Go to main display (Status)*/
	APP_voidScreenSwitch(APP_DISPLAY_STATUS,APP_SUB_DISPLAY_MAIN);

	/*Reset timer0*/
	APP_voidTimer0Reset();
}

void APP_voidStart(void)
{
	uint8 u8BuzzerCntrLoc = 0;
	uint8 u8AdcFlagLoc = 0;

	while(1)
	{
		/*If ADC flag = 1, start reading*/
		if(strFlagsGlb.u8AdcStartFlag==1)
		{
			strFlagsGlb.u8AdcStartFlag=0;

			if(u8AdcFlagLoc==0) /*Temperature sensor (Channel 0)*/
			{
				ADC_enuReadAsynch(ADC_CHANNEL_A0);
				u8AdcFlagLoc=1;
			}
			else if(u8AdcFlagLoc==1) /*Light sensor (Channel 1)*/
			{
				ADC_enuReadAsynch(ADC_CHANNEL_A1);
				u8AdcFlagLoc=0;
			}
		}

		/*If button is pressed, take action and update screen*/
		if(strFlagsGlb.u8ButtonActionFlag==1)
		{
			strFlagsGlb.u8UpdateFlag=1;

			/*Button action*/
			APP_voidButtonAction();

			/*Update screen*/
			APP_voidScreenUpdate();

			strFlagsGlb.u8ButtonActionFlag=0;
		}

		/*Every 1 second, update timer and sensors*/
		if(strFlagsGlb.u8TimedUpdateFlag==1)
		{
			strFlagsGlb.u8TimedUpdateFlag=0;
			strFlagsGlb.u8UpdateFlag=0;

			APP_voidScreenUpdate();

			/*Timer*/
			if((u16TimerGlb==0)&&(strStatesGlb.u8TimerState==1)) /*If timer is enabled & timer = 0*/
			{
				/*Disable timer*/
				strStatesGlb.u8TimerState=0;

				/*Timer -> light action*/
				if(strTimerOptionsGlb.u8LightAction==1)
				{
					APP_voidLightOn();
				}
				else if(strTimerOptionsGlb.u8LightAction==2)
				{
					APP_voidLightOff();
				}
				strTimerOptionsGlb.u8LightAction=0;

				/*Timer -> fan action*/
				if(strTimerOptionsGlb.u8FanAction==1)
				{
					APP_voidFanOn();
				}
				else if(strTimerOptionsGlb.u8FanAction==2)
				{
					APP_voidFanOff();
				}
				strTimerOptionsGlb.u8FanAction=0;

				/*Timer -> buzzer action*/
				if(strTimerOptionsGlb.u8BuzzerAction==1)
				{
					APP_voidBuzzerOn();
				}
				else if(strTimerOptionsGlb.u8BuzzerAction==0)
				{
					/*nothing*/
				}
				strTimerOptionsGlb.u8BuzzerAction=0;

			}
			else
			{

			}

			/*If buzzer is on, toggle every 1 second for 6 seconds*/
			if(strStatesGlb.u8BuzzerState==1)
			{
				u8BuzzerCntrLoc++;
				APP_voidBuzzerToggle();
				if(u8BuzzerCntrLoc==6)
				{
					APP_voidBuzzerOff();
					u8BuzzerCntrLoc=0;
				}
			}

			/*Fan -> temperature sensor*/
			u8CurrentTempGlb = (uint8)((uint32)u16Adc0ReadGlb*256/1023);
			if((strSensorOptionsGlb.u8TimpUpper!=0)&&((strSensorOptionsGlb.u8TimpLower!=0)))
			{
				if((strStatesGlb.u8FanState==0)&&(u8CurrentTempGlb>strSensorOptionsGlb.u8TimpUpper))
				{
					APP_voidFanOn();
				}
				else if((strStatesGlb.u8FanState==1)&&(u8CurrentTempGlb<strSensorOptionsGlb.u8TimpLower))
				{
					APP_voidFanOff();
				}
			}

			/*Light -> light sensor*/
			u8CurrentLightGlb = (uint8)((uint32)u16Adc1ReadGlb*256/1023);
			if((strSensorOptionsGlb.u8LightUpper!=0)&&((strSensorOptionsGlb.u8LightLower!=0)))
			{
				if((strStatesGlb.u8LightState==1)&&(u8CurrentLightGlb>strSensorOptionsGlb.u8LightUpper))
				{
					APP_voidLightOff();
				}
				else if((strStatesGlb.u8LightState==0)&&(u8CurrentLightGlb<strSensorOptionsGlb.u8LightLower))
				{
					APP_voidLightOn();
				}
			}
		}
	}
}

static void APP_voidADC_ISR(void)
{
	static uint8 u8FlagLoc = 0;

	if(u8FlagLoc==0) /*Receive Temperature Reading*/
	{
		ADC_enuGetData(&u16Adc0ReadGlb);
		u8FlagLoc=1;
	}
	else if(u8FlagLoc==1) /*Receive Light Reading*/
	{
		ADC_enuGetData(&u16Adc1ReadGlb);
		u8FlagLoc=0;
	}
	else
	{

	}

}

static void APP_voidUSART_ISR(void)
{
	static uint8 u8DataReceiveLoc = 0;

	USART_enuGetData(&u8DataReceiveLoc);

	switch(u8DataReceiveLoc)
	{
	case 'u':
	case 'U':
		strFlagsGlb.u8Button1Flag = 1;
		strFlagsGlb.u8ButtonActionFlag=1;
		break;
	case 'd':
	case 'D':
		strFlagsGlb.u8Button2Flag = 1;
		strFlagsGlb.u8ButtonActionFlag=1;
		break;
	case 'e':
	case 'E':
		strFlagsGlb.u8Button3Flag = 1;
		strFlagsGlb.u8ButtonActionFlag=1;
		break;
	default:
		break;
	}
}

static void APP_voidTimer0Cmp_ISR(void)
{
	static uint32 u32OvfCntrLoc = 0;

	/*Timer reset*/
	if(strFlagsGlb.u8TimerResetFlag==1)
	{
		strFlagsGlb.u8TimerResetFlag=0;
		TIMER0_voidSetTimerValue(0);
		u32OvfCntrLoc=0;
	}

	u32OvfCntrLoc++;

	/*at ovf = 80 or 240 -> start ADC conversion flag*/
	if((u32OvfCntrLoc==80) || (u32OvfCntrLoc==240))
	{
		strFlagsGlb.u8AdcStartFlag=1;
	}
	else if(u32OvfCntrLoc==500) /*at ovf = 500 (1 sec) -> decrement timer, 1 second timer flag*/
	{
		if(strStatesGlb.u8TimerState==1)
		{
			u16TimerGlb--;
		}
		u32OvfCntrLoc = 0;
		strFlagsGlb.u8TimedUpdateFlag=1;
	}

	/*Check pressed buttons*/
	if(strFlagsGlb.u8Button1Flag || strFlagsGlb.u8Button2Flag || strFlagsGlb.u8Button3Flag)
	{
		strFlagsGlb.u8ButtonActionFlag=1;
	}
}

static void APP_voidTimer0Reset(void)
{
	strFlagsGlb.u8TimerResetFlag=1;
}

static void APP_voidInt0_ISR(void)
{
	strFlagsGlb.u8Button1Flag = 1;
}

static void APP_voidInt1_ISR(void)
{
	strFlagsGlb.u8Button2Flag = 1;
}

static void APP_voidInt2_ISR(void)
{
	strFlagsGlb.u8Button3Flag = 1;
}

static void APP_voidPinCheck(const uint16 u16PinCodeCpy)
{
	uint8 u8PinFlagLoc = 0;
	uint8 u8ChoiceLoc=0;
	uint8 u8InputNumLoc = 0;
	uint16 u16TempPinLoc = 0;
	uint8 au8StringLoc[] ="Enter the PIN:";

	CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	CLCD_enuWriteString(au8StringLoc);
	CLCD_enuGotoxy(1,0);
	CLCD_enuWriteString("0000");

	/*Clear Button flags*/
	strFlagsGlb.u8Button1Flag = 0;
	strFlagsGlb.u8Button2Flag = 0;
	strFlagsGlb.u8Button3Flag = 0;
	strFlagsGlb.u8ButtonActionFlag=0;

	while(u8PinFlagLoc==0)
	{
		CLCD_enuGotoxy(1,u8InputNumLoc);
		if(strFlagsGlb.u8ButtonActionFlag==1) /*Up button pressed*/
		{
			strFlagsGlb.u8ButtonActionFlag=0;
			if(strFlagsGlb.u8Button1Flag==1)
			{
				strFlagsGlb.u8Button1Flag=0;

				if(u8ChoiceLoc==9)
				{
					u8ChoiceLoc=0;
				}
				else
				{
					u8ChoiceLoc++;
				}
				CLCD_enuWriteChar('0'+u8ChoiceLoc);
				CLCD_enuGotoxy(1,u8InputNumLoc);
			}
			else if(strFlagsGlb.u8Button2Flag==1) /*Down button pressed*/
			{
				strFlagsGlb.u8Button2Flag=0;
				if(u8ChoiceLoc==0)
				{
					u8ChoiceLoc=9;
				}
				else
				{
					u8ChoiceLoc--;
				}

				CLCD_enuWriteChar('0'+u8ChoiceLoc);
				CLCD_enuGotoxy(1,u8InputNumLoc);
			}
			else if(strFlagsGlb.u8Button3Flag==1) /*Enter button pressed*/
			{
				strFlagsGlb.u8Button3Flag=0;

				u16TempPinLoc=u16TempPinLoc*10+u8ChoiceLoc;

				if(u8InputNumLoc==3) /*User entered 4 numbers -> check PIN*/
				{
					if(u16TempPinLoc==u16PinCodeCpy) /*Correct PIN*/
					{
						u8PinFlagLoc=1;
					}
					else /*Wrong PIN*/
					{
						CLCD_enuGotoxy(0,0);
						CLCD_enuWriteString("Wrong,try again");
						CLCD_enuGotoxy(1,0);
						CLCD_enuWriteString("0000");
						CLCD_enuGotoxy(1,0);
						u8ChoiceLoc=0;
						u8InputNumLoc=0;
						u16TempPinLoc=0;
					}
				}
				else
				{
					u8InputNumLoc++;
					u8ChoiceLoc=0;
				}
			}
		}


	}
}
/****************************************/
