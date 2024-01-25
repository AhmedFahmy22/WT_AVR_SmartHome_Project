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

#ifndef APP_SCREEN_UPDATE_H_
#define APP_SCREEN_UPDATE_H_

/**********Function Declarations**********/
tenuErrorStatus APP_enuScreenUpdate(void);

void APP_voidDisplayTime(uint16 u16TimeCpy);
/****************************************/

#endif /* APP_SCREEN_UPDATE_H_ */
