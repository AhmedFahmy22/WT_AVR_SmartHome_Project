/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT1                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT1_INTERFACE_H_
#define EXTINT1_INTERFACE_H_

void EXTINT1_voidInit(void);

void EXTINT1_voidEnable(void);

void EXTINT1_voidDisable(void);

tenuErrorStatus EXTINT1_enuSetMode(const uint8 u8ModeCpy);

tenuErrorStatus EXTINT1_enuSetCallBack(const pf pfExtint1FunCpy);

#endif
