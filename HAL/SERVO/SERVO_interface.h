/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : SERVO                                               */
/* Version : 1.0                                                 */
/* Date    : 14 Jan 2024                                         */
/*****************************************************************/
#ifndef SERVO_INTERFACE_H_
#define SERVO_INTERFACE_H_

void SERVO_voidInit(void);

tenuErrorStatus SERVO_enuGotoAngle(const uint8 u8AngleCpy);

tenuErrorStatus SERVO_enuGetAngle(uint8* const pu8AngleCpy);

#endif
