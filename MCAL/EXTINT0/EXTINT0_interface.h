/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT0                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT0_INTERFACE_H_
#define EXTINT0_INTERFACE_H_

void EXTINT0_voidInit(void);

void EXTINT0_voidEnable(void);

void EXTINT0_voidDisable(void);

tenuErrorStatus EXTINT0_enuSetMode(const uint8 u8ModeCpy);

tenuErrorStatus EXTINT0_enuSetCallBack(const pf pfExtint0FunCpy);

#endif
