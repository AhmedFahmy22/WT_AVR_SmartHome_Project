/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : USART                                               */
/* Version : 1.0                                                 */
/* Date    : 25 Dec 2023                                         */
/*****************************************************************/
#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

void USART_voidInit(void);

tenuErrorStatus USART_enuReceiveSynch(uint16* const pu16DataCpy);

tenuErrorStatus USART_enuTransmitSynch(const uint16 u16DataCpy);

tenuErrorStatus USART_enuGetData(uint16* const pu16DataCpy);

tenuErrorStatus USART_enuSetData(const uint16 u16DataCpy);

tenuErrorStatus USART_enuReceiveSetCallBack(pf pfReceiveISRCpy);

tenuErrorStatus USART_enuTransmitSetCallBack(pf pfTransmitISRCpy);

#endif
