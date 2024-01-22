/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT2                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT2_INTERFACE_H_
#define EXTINT2_INTERFACE_H_

void EXTINT2_voidInit(void);

void EXTINT2_voidEnable(void);

void EXTINT2_voidDisable(void);

tenuErrorStatus EXTINT2_enuSetMode(const uint8 u8ModeCpy);

tenuErrorStatus EXTINT2_enuSetCallBack(const pf pfExtint2FunCpy);

#endif
