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

#ifndef APP_BUTTON_ACTION_H_
#define APP_BUTTON_ACTION_H_

/**********Function Declarations**********/
void APP_voidButtonAction(void);

void APP_voidLightOn(void);
void APP_voidLightOff(void);

void APP_voidFanOn(void);
void APP_voidFanOff(void);

void APP_voidBuzzerOn(void);
void APP_voidBuzzerOff(void);
void APP_voidBuzzerToggle(void);
/****************************************/

#endif /* APP_BUTTON_ACTION_H_ */
