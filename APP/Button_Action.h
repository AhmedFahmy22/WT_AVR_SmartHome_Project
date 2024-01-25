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
tenuErrorStatus APP_enuButtonAction(void);

tenuErrorStatus APP_enuLightOn(void);
tenuErrorStatus APP_enuLightOff(void);

tenuErrorStatus APP_enuFanOn(void);
tenuErrorStatus APP_enuFanOff(void);

void APP_voidBuzzerOn(void);
void APP_voidBuzzerOff(void);
tenuErrorStatus APP_enuBuzzerToggle(void);
/****************************************/

#endif /* APP_BUTTON_ACTION_H_ */
