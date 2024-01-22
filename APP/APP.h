/*
 * APP.h
 *
 *  Created on: Jan 9, 2024
 *      Author: Ahmed
 */

#ifndef APP_APP_H_
#define APP_APP_H_

/****************Typedefs****************/
typedef enum {
    APP_DISPLAY_STATUS,
    APP_DISPLAY_TIMER,
	APP_DISPLAY_FAN,
	APP_DISPLAY_LED,
	APP_DISPLAY_SERVO,
	APP_DISPLAY_PIN_CHANGE,
	APP_DISPLAY_NA
}tenuMainDisplay;

typedef enum {
    APP_SUB_DISPLAY_MAIN,
	APP_SUB_DISPLAY_1,
    APP_SUB_DISPLAY_2,
	APP_SUB_DISPLAY_3,
	APP_SUB_DISPLAY_4,
	APP_SUB_DISPLAY_NA
}tenuSubDisplay;

typedef struct {
	uint8 u8LightState  :1;
	uint8 u8FanState    :1;
	uint8 u8BuzzerState :1;
	uint8 u8TimerState  :1;
	uint8 u8ReservedBits :4;
	uint8 u8ServoState;
}tstrStates;

typedef struct {
	uint8 u8ButtonActionFlag :1;
	uint8 u8Button1Flag 	 :1;
	uint8 u8Button2Flag 	 :1;
	uint8 u8Button3Flag 	 :1;
	uint8 u8UpdateFlag		 :1;
	uint8 u8TimedUpdateFlag	 :1;
	uint8 u8AdcStartFlag	 :1;
	uint8 u8TimerResetFlag	 :1;
}tstrFlags;

typedef struct {
	uint8 u8LightAction  :2;
	uint8 u8FanAction	 :2;
	uint8 u8BuzzerAction :2;
	uint8 u8ReservedBits :2;
}tstrTimerOptions;

typedef struct {
	uint8 u8TimpUpper;
	uint8 u8TimpLower;
	uint8 u8LightUpper;
	uint8 u8LightLower;
}tstrSensorOptions;
/****************************************/

/******Global Variables Declarations******/
extern tstrFlags  strFlagsGlb;
extern tstrStates strStatesGlb;
extern tstrTimerOptions strTimerOptionsGlb;
extern tstrSensorOptions strSensorOptionsGlb;

extern uint16 u16CurrentPinGlb;
extern uint32 u32Tim0OvfCntrGlb;
extern uint8 u8CurrentTempGlb;
extern uint8 u8CurrentLightGlb;
extern uint16 u16TimerGlb;
extern tenuMainDisplay enuCurrentMainDisplayGlb;
extern tenuSubDisplay enuCurrentSubDisplayGlb;
extern uint8 u8ChoiceGlb;
extern uint8 u8InputNumGlb;
/****************************************/

/**********Function Declarations**********/
void APP_voidInit(void);

void APP_voidStart(void);
/****************************************/

#endif /* APP_APP_H_ */
