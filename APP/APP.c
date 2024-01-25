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
static void APP_voidTimer0Reset(void);
static void APP_voidExtInt0_ISR(void);
static void APP_voidExtInt1_ISR(void);
static void APP_voidExtInt2_ISR(void);
static tenuErrorStatus APP_enuPinCheck(const uint16 u16PinCodeCpy);
/****************************************/

/************Global Variables***********/
tstrFlags  strFlagsGlb;
tstrStates strStatesGlb;
tstrTimerOptions strTimerOptionsGlb;
tstrSensorOptions strSensorOptionsGlb;

static tenuErrorStatus enuErrorStatGlb = E_OK;

uint16 u16Adc0ReadGlb;
uint16 u16Adc1ReadGlb;
uint16 u16TimerGlb;
tenuMainDisplay enuCurrentMainDisplayGlb = APP_DISPLAY_NA;
tenuSubDisplay enuCurrentSubDisplayGlb = APP_DISPLAY_NA;
uint8 u8ChoiceGlb = 0;
uint8 u8InputNumGlb = 0;
uint8 u8CurrentTempGlb;
uint8 u8CurrentLightGlb;
uint16 u16CurrentPinGlb = 0;
/****************************************/

/**********Functions Definitions**********/
void APP_voidInit(void)
{
	uint8 u8PinStoreCheckLoc = 0xFF;

	/*DIO*/
	DIO_voidInit();
	enuErrorStatGlb |= DIO_enuPullUpEnable(DIO_PORTD, DIO_PIN2);
	enuErrorStatGlb |= DIO_enuPullUpEnable(DIO_PORTD, DIO_PIN3);
	enuErrorStatGlb |= DIO_enuPullUpEnable(DIO_PORTB, DIO_PIN2);

	/*EXEEPROM*/
	EXTEEPROM_voidInit();

	/*ADC*/
	ADC_voidInit();
	enuErrorStatGlb |= ADC_enuSetCallBack(APP_voidADC_ISR);

	/*UART*/
	USART_voidInit();
	enuErrorStatGlb |= USART_enuReceiveSetCallBack(APP_voidUSART_ISR);

	/*Timer0*/
	TIMER0_voidInit();
	TIMER0_voidSetCmpMatchValue(250);
	enuErrorStatGlb |= TIMER0_enuSetCallBackCmpMatch(APP_voidTimer0Cmp_ISR);
	TIMER0_voidIntCmpMatchEnable();

	/*External interrupts (Buttons)*/
	EXTINT0_voidInit();
	enuErrorStatGlb |= EXTINT0_enuSetCallBack(APP_voidExtInt0_ISR);
	EXTINT1_voidInit();
	enuErrorStatGlb |= EXTINT1_enuSetCallBack(APP_voidExtInt1_ISR);
	EXTINT2_voidInit();
	enuErrorStatGlb |= EXTINT2_enuSetCallBack(APP_voidExtInt2_ISR);

	/*Servo motor*/
	SERVO_voidInit();

	/*Enable interrupts*/
	ADC_voidInterruptEnable();
	EXTINT0_voidEnable();
	EXTINT1_voidEnable();
	EXTINT2_voidEnable();

	GIE_voidEnable();

	/*LCD*/
	enuErrorStatGlb |= CLCD_enuInit();

	/*Write / Read PIN from EEPROM*/
	enuErrorStatGlb |= EXTEEPROM_enuRead(0x50,&u8PinStoreCheckLoc);
	if(u8PinStoreCheckLoc==0xFF) /*If location == 0xFF, write default PIN : 0000*/
	{
		enuErrorStatGlb |= EXTEEPROM_enuWrite(0x50,0);
		enuErrorStatGlb |= EXTEEPROM_enuWrite(0x51,(uint8)u16CurrentPinGlb);
		enuErrorStatGlb |= EXTEEPROM_enuWrite(0x52,(uint8)(u16CurrentPinGlb>>8));
	}
	else /*Get stored PIN*/
	{
		enuErrorStatGlb |= EXTEEPROM_enuRead(0x51,(uint8*)(&u16CurrentPinGlb));
		enuErrorStatGlb |= EXTEEPROM_enuRead(0x52,(uint8*)(&u16CurrentPinGlb)+1);
	}


	/*Get and check PIN from user*/
	enuErrorStatGlb |= APP_enuPinCheck(u16CurrentPinGlb);

	/*Go to main display (Status)*/
	enuErrorStatGlb |= APP_enuScreenSwitch(APP_DISPLAY_STATUS,APP_SUB_DISPLAY_MAIN);

	/*Reset timer0*/
	APP_voidTimer0Reset();
}

void APP_voidStart(void)
{
	uint8 u8BuzzerCntrLoc = 0;

	while(1)
	{
		/*If button is pressed, take action and update screen*/
		if(strFlagsGlb.u8ButtonActionFlag==1)
		{
			strFlagsGlb.u8UpdateFlag=1;

			/*Button action*/
			enuErrorStatGlb |= APP_enuButtonAction();

			/*Update screen*/
			enuErrorStatGlb |= APP_enuScreenUpdate();

			/*delay for bouncing effect*/
			TIMER0_voidDelay(100); /*100 overflows -> 200 ms*/

			strFlagsGlb.u8Button1Flag = 0;
			strFlagsGlb.u8Button2Flag = 0;
			strFlagsGlb.u8Button3Flag = 0;
			strFlagsGlb.u8ButtonActionFlag=0;
		}

		/*Every 1 second, update timer and sensors*/
		if(strFlagsGlb.u8TimedUpdateFlag==1)
		{
			strFlagsGlb.u8TimedUpdateFlag=0;
			strFlagsGlb.u8UpdateFlag=0;

			enuErrorStatGlb |= APP_enuScreenUpdate();

			/*Timer*/
			if((u16TimerGlb==0)&&(strStatesGlb.u8TimerState==1)) /*If timer is enabled & timer = 0*/
			{
				/*Disable timer*/
				strStatesGlb.u8TimerState=0;

				/*Timer -> light action*/
				if(strTimerOptionsGlb.u8LightAction==1)
				{
					enuErrorStatGlb |= APP_enuLightOn();
				}
				else if(strTimerOptionsGlb.u8LightAction==2)
				{
					enuErrorStatGlb |= APP_enuLightOff();
				}
				strTimerOptionsGlb.u8LightAction=0;

				/*Timer -> fan action*/
				if(strTimerOptionsGlb.u8FanAction==1)
				{
					enuErrorStatGlb |= APP_enuFanOn();
				}
				else if(strTimerOptionsGlb.u8FanAction==2)
				{
					enuErrorStatGlb |= APP_enuFanOff();
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
				enuErrorStatGlb |= APP_enuBuzzerToggle();
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
					enuErrorStatGlb |= APP_enuFanOn();
				}
				else if((strStatesGlb.u8FanState==1)&&(u8CurrentTempGlb<strSensorOptionsGlb.u8TimpLower))
				{
					enuErrorStatGlb |= APP_enuFanOff();
				}
			}

			/*Light -> light sensor*/
			u8CurrentLightGlb = (uint8)((uint32)u16Adc1ReadGlb*100/1023);
			if((strSensorOptionsGlb.u8LightUpper!=0)&&((strSensorOptionsGlb.u8LightLower!=0)))
			{
				if((strStatesGlb.u8LightState==1)&&(u8CurrentLightGlb>strSensorOptionsGlb.u8LightUpper))
				{
					enuErrorStatGlb |= APP_enuLightOff();
				}
				else if((strStatesGlb.u8LightState==0)&&(u8CurrentLightGlb<strSensorOptionsGlb.u8LightLower))
				{
					enuErrorStatGlb |= APP_enuLightOn();
				}
			}

			/*Error LED*/
			if(enuErrorStatGlb!=E_OK)
			{
				DIO_enuWritePinValue(DIO_PORTA, DIO_PIN5, DIO_HIGH);
			}
			else
			{

			}
		}
	}
}

static void APP_voidADC_ISR(void)
{
	if(strFlagsGlb.u8AdcChannelFlag==0) /*Receive Temperature Reading*/
	{
		enuErrorStatGlb |= ADC_enuGetData(&u16Adc0ReadGlb);
	}
	else if(strFlagsGlb.u8AdcChannelFlag==1) /*Receive Light Reading*/
	{
		enuErrorStatGlb |= ADC_enuGetData(&u16Adc1ReadGlb);
	}
	else
	{

	}
}

static void APP_voidUSART_ISR(void)
{
	static uint8 u8DataReceiveLoc = 0;

	enuErrorStatGlb |= USART_enuGetData(&u8DataReceiveLoc);

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
	if(u32OvfCntrLoc==80)
	{
		/*Start conversion on channel 0 (Temperature sensor)*/
		enuErrorStatGlb |= ADC_enuReadAsynch(ADC_CHANNEL_A0);
		strFlagsGlb.u8AdcChannelFlag=0;
	}
	else if(u32OvfCntrLoc==240)
	{
		/*Start conversion on channel 1 (Light sensor)*/
		enuErrorStatGlb |= ADC_enuReadAsynch(ADC_CHANNEL_A1);
		strFlagsGlb.u8AdcChannelFlag=1;
	}
	else if(u32OvfCntrLoc==500) /*at ovf = 500 (1 sec)*/
	{
		/*Decrement timer*/
		if(strStatesGlb.u8TimerState==1)
		{
			u16TimerGlb--;
		}
		u32OvfCntrLoc = 0;

		/*1 second update flag*/
		strFlagsGlb.u8TimedUpdateFlag=1;
	}
}

static void APP_voidTimer0Reset(void)
{
	strFlagsGlb.u8TimerResetFlag=1;
}

static void APP_voidExtInt0_ISR(void)
{
	strFlagsGlb.u8Button1Flag = 1;
	strFlagsGlb.u8ButtonActionFlag=1;
}

static void APP_voidExtInt1_ISR(void)
{
	strFlagsGlb.u8Button2Flag = 1;
	strFlagsGlb.u8ButtonActionFlag=1;
}

static void APP_voidExtInt2_ISR(void)
{
	strFlagsGlb.u8Button3Flag = 1;
	strFlagsGlb.u8ButtonActionFlag=1;
}

static tenuErrorStatus APP_enuPinCheck(const uint16 u16PinCodeCpy)
{
	tenuErrorStatus enuErrorStatLoc = E_OK;
	uint8 u8PinFlagLoc = 0;
	uint8 u8ChoiceLoc=0;
	uint8 u8InputNumLoc = 0;
	uint16 u16TempPinLoc = 0;
	uint8 au8StringLoc[] ="Enter the PIN:";

	enuErrorStatLoc |= CLCD_enuSendCommand(CLCD_COMM_CURSOR_ON);
	enuErrorStatLoc |= CLCD_enuWriteString(au8StringLoc);
	enuErrorStatLoc |= CLCD_enuGotoxy(1,0);
	enuErrorStatLoc |= CLCD_enuWriteString("0000");

	/*Clear Button flags*/
	strFlagsGlb.u8Button1Flag = 0;
	strFlagsGlb.u8Button2Flag = 0;
	strFlagsGlb.u8Button3Flag = 0;
	strFlagsGlb.u8ButtonActionFlag=0;

	while(u8PinFlagLoc==0)
	{
		enuErrorStatLoc |= CLCD_enuGotoxy(1,u8InputNumLoc);
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
				enuErrorStatLoc |= CLCD_enuWriteChar('0'+u8ChoiceLoc);
				enuErrorStatLoc |= CLCD_enuGotoxy(1,u8InputNumLoc);
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

				enuErrorStatLoc |= CLCD_enuWriteChar('0'+u8ChoiceLoc);
				enuErrorStatLoc |= CLCD_enuGotoxy(1,u8InputNumLoc);
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
						enuErrorStatLoc |= CLCD_enuGotoxy(0,0);
						enuErrorStatLoc |= CLCD_enuWriteString("Wrong,try again");
						enuErrorStatLoc |= CLCD_enuGotoxy(1,0);
						enuErrorStatLoc |= CLCD_enuWriteString("0000");
						enuErrorStatLoc |= CLCD_enuGotoxy(1,0);
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
	return enuErrorStatLoc;
}
/****************************************/
